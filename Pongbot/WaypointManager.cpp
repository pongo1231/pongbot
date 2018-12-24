#include "WaypointManager.h"
#include "Util.h"
#include "WaypointNode.h"
#include "WaypointFileManager.h"
#include <metamod/ISmmPlugin.h>
#include <metamod/sourcehook.h>
#include <map>

#define WAYPOINT_NODE_BEAM_TICK .5f
#define WAYPOINT_NODE_BEAM_DRAWDIST 1000.f

extern IVEngineServer *Engine;
extern IPlayerInfoManager *IIPlayerInfoManager;

WaypointManager *_WaypointManager;

static std::vector<WaypointNode*> _WaypointNodes;
static WaypointNode *_SelectedNode;
static bool _NodeBiConnect;
static bool _DrawBeams;

WaypointManager::WaypointManager()
{}

void WaypointManager::Init()
{
	Assert(!_WaypointManager);

	_WaypointNodes.clear();

	WaypointFileManager::Init(&_WaypointNodes);

	_SelectedNode = nullptr;
	_NodeBiConnect = false;
	_DrawBeams = false;
	_WaypointManager = new WaypointManager();
}

void WaypointManager::Destroy()
{
	Assert(_WaypointManager);

	WaypointFileManager::Destroy();

	for (uint8_t i = 0; i < _WaypointNodes.size(); i++)
	{
		delete _WaypointNodes[i];
		_WaypointNodes.erase(_WaypointNodes.begin() + i);
	}

	delete _WaypointManager;
}

WaypointNode *WaypointManager::GetRandomWaypointNode() const
{
	if (_WaypointNodes.empty())
		return nullptr;

	return _WaypointNodes[Util::RandomInt(0, _WaypointNodes.size() - 1)];
}

WaypointNode *WaypointManager::GetClosestWaypointNode(Vector pos) const
{
	WaypointNode *closestNode = nullptr;
	float closestDistance = 9999.f; // Just something insanely high

	for (WaypointNode *node : _WaypointNodes)
	{
		float distance = node->Pos.DistTo(pos);

		if (closestDistance > distance)
		{
			closestNode = node;
			closestDistance = distance;
		}
	}

	return closestNode;
}

vec_t WaypointManager::GetShortestWaypointNodeRouteToTargetNode(WaypointNode *startNode,
	WaypointNode *targetNode, std::stack<WaypointNode*> *waypointNodesStack,
	std::vector<WaypointNode*> *_alreadyTraversedWaypointNodesStack)
{
	if (!startNode || !targetNode || !waypointNodesStack)
		return -1;
	if (startNode == targetNode)
		return 0.f;
	if (!_alreadyTraversedWaypointNodesStack)
		_alreadyTraversedWaypointNodesStack = &std::vector<WaypointNode*>();

	// Check if this node was already traversed to avoid infinite recursive calls
	for (WaypointNode *node : *_alreadyTraversedWaypointNodesStack)
		if (node == startNode)
			return -1;
	_alreadyTraversedWaypointNodesStack->push_back(startNode);

	std::map<WaypointNode*, vec_t> distances;
	for (WaypointNode *node : *startNode->GetConnectedNodes())
	{
		vec_t distance = GetShortestWaypointNodeRouteToTargetNode(node, targetNode,
			waypointNodesStack, _alreadyTraversedWaypointNodesStack);
		// Distance under 0 = no connetion to target node
		// Distance of 0 = Target Node
		if (distance >= 0.f)
			distances.insert(std::make_pair(node, distance));
	}

	if (!distances.empty())
	{
		/* Return path with shortest distance */
		WaypointNode *closestDistanceNode = nullptr;
		vec_t closestDistance = 99999999;
		for (auto const& pair : distances)
			if (pair.second /* distance */ < closestDistance)
			{
				closestDistanceNode = pair.first /* node */;
				closestDistance = pair.second;
			}

		waypointNodesStack->push(closestDistanceNode);
		return closestDistance + closestDistanceNode->Pos.DistTo(startNode->Pos);
	}

	return -1;
}

/// Debug
/// Draw beams for each waypoint & their connections
void WaypointManager::OnGameFrame()
{
	if (!_DrawBeams)
		return;

	static float tickTime;
	float currentTime = Engine->Time();
	if (tickTime > currentTime)
		return;
	tickTime = currentTime + WAYPOINT_NODE_BEAM_TICK;

	edict_t *drawPlayerEdict = Engine->PEntityOfEntIndex(1);
	IPlayerInfo *drawPlayerInfo = IIPlayerInfoManager->GetPlayerInfo(drawPlayerEdict);
	Vector playerPos = drawPlayerInfo->GetAbsOrigin();
	if (drawPlayerEdict && drawPlayerInfo && drawPlayerInfo->IsPlayer())
	{
		std::vector<WaypointNode*> drawnNodes;
		for (WaypointNode *node : _WaypointNodes)
		{
			bool alreadyDrawn = false;
			for (WaypointNode *drawnNode : drawnNodes)
				if (drawnNode == node)
				{
					alreadyDrawn = true;
					break;
				}
			if (!alreadyDrawn)
			{
				Vector startPos = node->Pos;
				if (startPos.DistTo(playerPos) <= WAYPOINT_NODE_BEAM_DRAWDIST)
				{
					Vector endPos = startPos + Vector(0.f, 0.f, 75.f);
					Util::DrawBeam(startPos, endPos, 0, 255, 0, WAYPOINT_NODE_BEAM_TICK);
					for (WaypointNode *connectedNode : *node->GetConnectedNodes())
					{
						Util::DrawBeam(endPos, connectedNode->Pos, 255, 255, 255, WAYPOINT_NODE_BEAM_TICK);

						alreadyDrawn = false;
						for (WaypointNode *drawnNode : drawnNodes)
							if (drawnNode == connectedNode)
							{
								alreadyDrawn = true;
								break;
							}
						if (!alreadyDrawn)
						{
							startPos = connectedNode->Pos;
							Util::DrawBeam(startPos, startPos + Vector(0.f, 0.f, 75.f), 0, 255, 0, WAYPOINT_NODE_BEAM_TICK);

							drawnNodes.push_back(node);
						}
					}

					drawnNodes.push_back(node);
				}
			}
		}
	}
}

static IPlayerInfo *_CheckCommandTargetPlayerExists()
{
	edict_t *playerEdict = Engine->PEntityOfEntIndex(1);
	IPlayerInfo *playerInfo = IIPlayerInfoManager->GetPlayerInfo(playerEdict);
	if (!playerEdict || !playerInfo || !playerInfo->IsPlayer())
	{
		Util::Log("No player found!");
		return nullptr;
	}
	return playerInfo;
}

CON_COMMAND(pongbot_waypoint_createnode, "Creates a waypoint node wherever the first player is standing")
{
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo)
	{
		uint8_t id = _WaypointNodes.size();
		if (id >= 256) // Above max size of 8 bit (255)
			Util::Log("Max amount of waypoint nodes reached (255)!");
		else
		{
			_WaypointNodes.push_back(new WaypointNode(id, playerInfo->GetAbsOrigin()));

			Util::Log("Created waypoint node #%d", id);
		}
	}
}

CON_COMMAND(pongbot_waypoint_connectnode1, "Selects nearest waypoint node for connection with another node")
{
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo)
	{
		_SelectedNode = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
		if (!_SelectedNode)
			Util::Log("No waypoint node found!");
		else
			Util::Log("Waypoint node #%d selected", _SelectedNode->Id);
	}
}

CON_COMMAND(pongbot_waypoint_connectnode2, "Connects previously selected waypoint node with nearest node")
{
	if (!_SelectedNode)
		Util::Log("Select a node via pongbot_connectnode1 first");
	else
	{
		IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
		if (playerInfo)
		{
			WaypointNode *currentNode = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
			if (_SelectedNode == currentNode)
				Util::Log("Can't connect waypoint node to itself!");
			else
			{
				int selectedNodeID = _SelectedNode->Id;
				int currentNodeID = currentNode->Id;

				if (!_SelectedNode->ConnectToNode(currentNode, _NodeBiConnect))
					Util::Log("Node #%d and #%d were already connected!", selectedNodeID, currentNodeID);
				else
				{
					Util::Log("Connected waypoint node #%d with node #%d", selectedNodeID, currentNodeID);
					_SelectedNode = nullptr;
				}
			}
		}
	}
}

CON_COMMAND(pongbot_waypoint_biconnect, "Toggles automatic node bidirectional connections")
{
	_NodeBiConnect = !_NodeBiConnect;
	if (_NodeBiConnect)
		Util::Log("Bidirectional node connections enabled!");
	else
		Util::Log("Bidirectional node connections disabled!");
}

CON_COMMAND(pongbot_waypoint_clearnodes, "Removes all waypoint nodes")
{
	for (WaypointNode *node : _WaypointNodes)
		delete node;
	_WaypointNodes.clear();
	Util::Log("All waypoint nodes cleared!");
}

CON_COMMAND(pongbot_waypoint_clearnode, "Removes the nearest node")
{
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo)
	{
		WaypointNode *node = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
		if (!node)
			Util::Log("No waypoint node found!");
		else
		{
			// IMPORTANT TODO: Also remove from other nodes' connected lists
			// Otherwise there will be issues when saving!!!
			
			// Delete from list first before deleting completely
			for (uint8_t i = 0; i < _WaypointNodes.size(); i++)
				if (_WaypointNodes[i] == node) 
					_WaypointNodes.erase(_WaypointNodes.begin() + i);
			delete node;

			Util::Log("Removed nearest node!");
		}
	}
}

CON_COMMAND(pongbot_waypoint_clearnodeto, "Clears all connections to other nodes from node")
{
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo)
	{
		WaypointNode *node = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
		if (!node)
			Util::Log("No waypoint node found!");
		else
		{
			std::vector<WaypointNode*> *connectedNodes = node->GetConnectedNodes();
			for (uint8_t i = 0; i < connectedNodes->size(); i++)
				connectedNodes->erase(connectedNodes->begin() + i);

			Util::Log("Cleared node connections of closest node!");
		}
	}
}

CON_COMMAND(pongbot_waypoint_debug, "Toggle beams to visualize nodes & their connections")
{
	_DrawBeams = !_DrawBeams;
	if (_DrawBeams)
		Util::Log("Waypoint Debugging enabled!");
	else
		Util::Log("Waypoint Debugging disabled!");
}

CON_COMMAND(pongbot_waypoint_getnodeid, "Outputs ID of closest node")
{
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo)
	{
		WaypointNode *node = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
		if (!node)
			Util::Log("No waypoint node found!");
		else
		{
			Util::Log("Node ID: %d", node->Id);
		}
	}
}