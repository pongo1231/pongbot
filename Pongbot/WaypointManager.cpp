#include "WaypointManager.h"
#include "Util.h"
#include "WaypointFileManager.h"
#include "WaypointNodeFlagsProvider.h"
#include "TraceFilters.h"
#include "ConVarHolder.h"
#include "Player.h"
#include <metamod/ISmmPlugin.h>
#include <metamod/sourcehook.h>
#include <hlsdk/public/mathlib/mathlib.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <map>

extern IVEngineServer *Engine;
extern IPlayerInfoManager *IIPlayerInfoManager;
extern IEngineTrace *IIEngineTrace;

WaypointManager *_WaypointManager;

static std::vector<WaypointNode*> _WaypointNodes;
static WaypointNode *_SelectedNode;
static bool _NodeBiConnect;
static bool _DrawBeams;

void WaypointManager::Init()
{
	if (!_WaypointManager)
	{
		Util::DebugLog("INIT WaypointManager");

		_WaypointNodes.clear();
		WaypointFileManager::Init(&_WaypointNodes);
		WaypointNodeFlagsProvider::Init();

		_SelectedNode = nullptr;
		_NodeBiConnect = false;
		_DrawBeams = false;
		_WaypointManager = new WaypointManager();
	}
}

void WaypointManager::Destroy()
{
	if (_WaypointManager)
	{
		Util::DebugLog("DESTROY WaypointManager");

		WaypointFileManager::Destroy();
		WaypointNodeFlagsProvider::Destroy();

		for (uint8_t i = 0; i < _WaypointNodes.size(); i++)
		{
			delete _WaypointNodes[i];
			_WaypointNodes.erase(_WaypointNodes.begin() + i);
		}
		delete _WaypointManager;
	}
}

WaypointNode *WaypointManager::GetRandomWaypointNode(unsigned int nodeFlagBlacklist) const
{
	if (!_WaypointNodes.empty())
	{
		if (nodeFlagBlacklist == 0) // Don't waste resources unnecessarily, doofus
			return _WaypointNodes[Util::RandomInt(0, _WaypointNodes.size() - 1)];
		else
		{
			std::vector<WaypointNode*> waypointNodes = _WaypointNodes;
			while (!waypointNodes.empty())
			{
				int randomNodeIndex = Util::RandomInt(0, waypointNodes.size() - 1);
				WaypointNode *randomNode = waypointNodes[randomNodeIndex];
				if (randomNode->Flags & nodeFlagBlacklist)
					waypointNodes.erase(waypointNodes.begin() + randomNodeIndex);
				else
					return randomNode;
			}
		}
	}

	return nullptr;
}

WaypointNode *WaypointManager::GetClosestWaypointNode(Vector pos, float maxDistance, unsigned int nodeFlagWhitelist) const
{
	WaypointNode *closestNode = nullptr;
	float closestDistance = maxDistance > 0.f ? maxDistance : 99999.f;
	for (WaypointNode *node : _WaypointNodes)
	{
		float distance = node->Pos.DistTo(pos);
		if (closestDistance > distance && (nodeFlagWhitelist == 0 || node->Flags & nodeFlagWhitelist))
		{
			// Do raytracing to check if node is reachable
			/*Vector nodeRayTracePos = node->Pos;
			nodeRayTracePos.z += 75.f;
			Ray_t traceLine;
			traceLine.Init(pos + 1.f \/* To be sure it's not inside the ground *\/, nodeRayTracePos);
			trace_t traceResult;
			IIEngineTrace->TraceRay(traceLine, MASK_SOLID, &TraceFilterWorld(), &traceResult);
			if (!traceResult.DidHit())
			{*/
				closestNode = node;
				closestDistance = distance;
			/*}*/
		}
	}

	return closestNode;
}

float WaypointManager::GetShortestWaypointNodeRouteToTargetNode(WaypointNode *startNode,
	WaypointNode *targetNode, std::stack<WaypointNode*> *waypointNodesStack, unsigned int flagBlacklist,
	std::vector<WaypointNode*> *_alreadyTraversedWaypointNodesStack)
{
	bool firstNode = false;
	if (!startNode || !targetNode || !waypointNodesStack
		|| (flagBlacklist != 0 && startNode->Flags & flagBlacklist /* Abort if flag is filtered */))
		return -1;
	else if (startNode == targetNode)
		return 0.f;
	else if (!_alreadyTraversedWaypointNodesStack)
	{
		/* Very likely first traversed node (unless _alreadyTraversedWaypointNodesStack was already provided) */
		firstNode = true;
		_alreadyTraversedWaypointNodesStack = &std::vector<WaypointNode*>();
	}

	// Check if this node was already traversed to avoid infinite recursive calls
	for (WaypointNode *node : *_alreadyTraversedWaypointNodesStack)
		if (node == startNode)
			return -1;
	_alreadyTraversedWaypointNodesStack->push_back(startNode);

	std::map<WaypointNode*, float> distances;
	for (WaypointNode *node : startNode->GetConnectedNodes())
	{
		float distance = GetShortestWaypointNodeRouteToTargetNode(node, targetNode,
			waypointNodesStack, flagBlacklist, _alreadyTraversedWaypointNodesStack);
		// Distance under 0 = no connetion to target node
		// Distance of 0 = Target Node
		if (distance >= 0.f)
			distances.insert(std::make_pair(node, distance));
	}

	if (!distances.empty())
	{
		/* Return path with shortest distance */
		WaypointNode *closestDistanceNode = nullptr;
		float closestDistance = 99999.f;
		for (auto const& pair : distances)
			if (pair.second /* distance */ < closestDistance)
			{
				closestDistanceNode = pair.first /* node */;
				closestDistance = pair.second;
			}

		waypointNodesStack->push(closestDistanceNode);
		return closestDistance + closestDistanceNode->Pos.DistTo(startNode->Pos);
	}

	if (firstNode)
		waypointNodesStack->push(startNode);

	return -1;
}

bool WaypointManager::GetRandomWaypointNodeRouteToTargetNode(WaypointNode *startNode,
	WaypointNode *targetNode, std::stack<WaypointNode*> *waypointNodesStack, unsigned int flagBlacklist,
	std::vector<WaypointNode*> *_alreadyTraversedWaypointNodesStack)
{
	if (!startNode || !targetNode || !waypointNodesStack
		|| (flagBlacklist != 0 && flagBlacklist & startNode->Flags /* Abort if flag is filtered */))
		return false;
	else if (startNode == targetNode)
	{
		waypointNodesStack->push(startNode);
		return true;
	}
	else if (!_alreadyTraversedWaypointNodesStack)
		_alreadyTraversedWaypointNodesStack = &std::vector<WaypointNode*>();

	// Check if this node was already traversed to avoid infinite recursive calls
	for (WaypointNode *node : *_alreadyTraversedWaypointNodesStack)
		if (node == startNode)
			return false;
	_alreadyTraversedWaypointNodesStack->push_back(startNode);

	std::vector<WaypointNode*> connectedNodes = startNode->GetConnectedNodes();
	while (!connectedNodes.empty())
	{
		int randomNodeIndex = Util::RandomInt(0, connectedNodes.size() - 1);
		WaypointNode *randomNode = connectedNodes[randomNodeIndex];
		if (GetRandomWaypointNodeRouteToTargetNode(randomNode, targetNode, waypointNodesStack, flagBlacklist,
			_alreadyTraversedWaypointNodesStack))
		{
			waypointNodesStack->push(startNode);
			return true;
		}
		else
			connectedNodes.erase(connectedNodes.begin() + randomNodeIndex);
	}

	return false;
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
	float debugBeamTick = _ConVarHolder->CVarWaypointNodeDebugBeamTick->GetFloat();
	tickTime = currentTime + debugBeamTick;

	Player firstPlayer(Engine->PEntityOfEntIndex(1));
	if (firstPlayer.Exists() && firstPlayer.IsPlayer())
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
				if (startPos.DistTo(firstPlayer.GetPos()) <= _ConVarHolder->CVarWaypointNodeDebugBeamDist->GetFloat())
				{
					Vector endPos = startPos + Vector(0.f, 0.f, 75.f);
					Util::DrawBeam(startPos, endPos, node->Flags != 0 ? 255 : 0, 255, 0, debugBeamTick);
					for (WaypointNode *connectedNode : node->GetConnectedNodes())
					{
						Util::DrawBeam(endPos, connectedNode->Pos, 255, 255, 255, debugBeamTick);

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
							Util::DrawBeam(startPos, startPos + Vector(0.f, 0.f, 75.f),
								connectedNode->Flags != 0 ? 255 : 0, 255, 0, debugBeamTick);

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
		// Check for first empty id
		for (uint8_t id = 0; id < 256; id++)
		{
			bool isIdFree = true;
			for (WaypointNode *node : _WaypointNodes)
				if (node->Id == id)
				{
					isIdFree = false;
					break;
				}

			if (isIdFree)
			{
				_WaypointNodes.push_back(new WaypointNode(id, playerInfo->GetAbsOrigin()));
				Util::Log("Created waypoint node #%d", id);
				break;
			}
			else if (id == 255 && !isIdFree)
				Util::Log("Max amount of waypoint nodes reached (255)!");
		}
	}
}

CON_COMMAND(pongbot_waypoint_connectnode1, "Selects nearest waypoint node for connection with another node")
{
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo)
	{
		_SelectedNode = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
		Util::Log(!_SelectedNode ? "No waypoint node found!" : "Waypoint node #%d selected", _SelectedNode->Id);
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
	Util::Log(_NodeBiConnect ? "Bidirectional node connections enabled!" : "Bidirectional node connections disabled!");
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
			// Delete from other nodes' connections and save target node's pos
			uint8_t nodeIndex = 0;
			for (uint8_t i = 0; i < _WaypointNodes.size(); i++)
			{
				WaypointNode *listNode = _WaypointNodes[i];
				if (listNode == node)
					nodeIndex = i;
				else
					listNode->UnconnectNode(node);
			}
			
			// Delete from nodes list
			_WaypointNodes.erase(_WaypointNodes.begin() + nodeIndex);

			uint8_t nodeId = node->Id;
			delete node;

			Util::Log("Cleared nearest node #%d!", nodeId);
		}
	}
}

CON_COMMAND(pongbot_waypoint_clearnodeconnections, "Clears all connections of node")
{
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo)
	{
		WaypointNode *node = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
		if (!node)
			Util::Log("No waypoint node found!");
		else
		{
			node->UnconnectAllNodes(true);
			Util::Log("Cleared node connections of closest node!");
		}
	}
}

CON_COMMAND(pongbot_waypoint_debug, "Toggle beams to visualize nodes & their connections")
{
	_DrawBeams = !_DrawBeams;
	Util::Log(_DrawBeams ? "Waypoint Debugging enabled!" : "Waypoint Debugging disabled!");
}

CON_COMMAND(pongbot_waypoint_getnodeid, "Outputs ID of closest node")
{
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo)
	{
		WaypointNode *node = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
		Util::Log(!node ? "No waypoint node found!" : "Node ID: %d", node->Id);
	}
}

CON_COMMAND(pongbot_waypoint_togglenodeflag, "Adds/Removes a flag to a waypoint node")
{
	std::map<WaypointNodeFlagType, WaypointNodeFlagInfo> nodeFlags = _WaypointNodeFlagsProvider->GetAllNodeFlags();
	const char *flagName = args[1];
	if (strcmp(flagName, "") == 0)
	{
		char buffer[512] = "Possible flags: ";
		for (auto const &pair : nodeFlags)
		{
			WaypointNodeFlagInfo info = pair.second;
			char flagInfoBuffer[128];
			sprintf(flagInfoBuffer, "\n%s - %s", info.Name, info.Desc);
			strcat(buffer, flagInfoBuffer);
		}
		Util::Log(buffer);
	}
	else
	{
		IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
		if (playerInfo)
		{
			WaypointNode *node = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
			if (!node)
				Util::Log("No waypoint node found!");
			else
			{
				bool validFlag = false;
				bool flagRemoved = false;
				for (auto const &pair : nodeFlags)
				{
					WaypointNodeFlagInfo info = pair.second;
					if (strcmpi(flagName, info.Name) == 0)
					{
						int flag = pair.first;
						if (node->Flags & flag)
						{
							node->Flags &= ~flag;
							flagRemoved = true;
						}
						else
							node->Flags |= flag;
						validFlag = true;
						break;
					}
				}

				if (validFlag)
					Util::Log(flagRemoved ? "Removed flag from node #%d" : "Added flag to node #%d", node->Id);
				else
					Util::Log("Invalid flag!");
			}
		}
	}
}

CON_COMMAND(pongbot_waypoint_getnodeflags, "Outputs all flags of a waypoint node")
{
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo)
	{
		WaypointNode *node = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
		if (!node)
			Util::Log("No waypoint node found!");
		else
		{
			char buffer[512];
			sprintf(buffer, "Node #%d has following flags:", node->Id);
			for (auto const &pair : _WaypointNodeFlagsProvider->GetAllNodeFlags())
				if (node->Flags & pair.first)
				{
					char flagNameBuffer[32];
					sprintf(flagNameBuffer, "\n%s", pair.second.Name);
					strcat(buffer, flagNameBuffer);
				}
			Util::Log(buffer);
		}
	}
}

CON_COMMAND(pongbot_waypoint_clearnodeflags, "Clears all flags of a waypoint node")
{
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo)
	{
		WaypointNode *node = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
		if (!node)
			Util::Log("No waypoint node found!");
		else
		{
			node->Flags = 0;
			Util::Log("Cleared flags of node #%d", node->Id);
		}
	}
}