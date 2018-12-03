#include "WaypointManager.h"
#include "Util.h"
#include "WaypointNode.h"
#include "WaypointFileManager.h"
#include <metamod/ISmmPlugin.h>
#include <metamod/sourcehook.h>

extern IVEngineServer *Engine;
extern IPlayerInfoManager *IIPlayerInfoManager;

WaypointManager *_WaypointManager;

static vector<WaypointNode*> _WaypointNodes;
static WaypointNode *_SelectedNode;

void WaypointManager::Init() {
	Assert(!_WaypointManager);
	_WaypointManager = new WaypointManager();
	WaypointFileManager::Init(&_WaypointNodes);
}

void WaypointManager::Destroy() {
	Assert(_WaypointManager);
	WaypointFileManager::Destroy();
	for (uint8_t i = 0; i < _WaypointNodes.size(); i++) {
		delete _WaypointNodes[i];
		_WaypointNodes.erase(_WaypointNodes.begin() + i);
	}
	delete _WaypointManager;
}

WaypointManager::WaypointManager() {}

WaypointNode *WaypointManager::GetRandomWaypointNode() const {
	if (_WaypointNodes.empty())
		return nullptr;
	return _WaypointNodes[Util::RandomInt(0, _WaypointNodes.size() - 1)];
}

WaypointNode *WaypointManager::GetClosestWaypointNode(Vector pos) const {
	WaypointNode *closestNode = nullptr;
	float closestDistance = 9999; // Just something insanely high
	for (WaypointNode *node : _WaypointNodes) {
		float distance = node->Pos.DistTo(pos);
		if (closestDistance > distance) {
			closestNode = node;
			closestDistance = distance;
		}
	}
	return closestNode;
}

bool WaypointManager::GetWaypointNodeStackToTargetNode(WaypointNode *startNode,
	WaypointNode *targetNode, stack<WaypointNode*> *waypointNodesStack,
	vector<WaypointNode*> *_alreadyTraversedWaypointNodesStack) {
	if (!startNode || !targetNode || !waypointNodesStack)
		return false;
	if (startNode == targetNode) {
		waypointNodesStack->push(startNode);
		return true;
	}
	else {
		// Check if this node was already traversed to avoid infinite recursive calls
		if (!_alreadyTraversedWaypointNodesStack)
			_alreadyTraversedWaypointNodesStack = &vector<WaypointNode*>();
		for (WaypointNode *node : *_alreadyTraversedWaypointNodesStack)
			if (node == startNode)
				return false;
		_alreadyTraversedWaypointNodesStack->push_back(startNode);

		for (WaypointNode *node : *startNode->GetConnectedNodes())
			if (GetWaypointNodeStackToTargetNode(node, targetNode, waypointNodesStack,
				_alreadyTraversedWaypointNodesStack)) {
				waypointNodesStack->push(startNode);
				return true;
			}
	}
	return false;
}

void WaypointManager::OnGameFrame() {
	// Draw beams for each waypoint & their connections
	for (WaypointNode *node : _WaypointNodes) {
		Vector startPos = node->Pos;
		Vector endPos = startPos;
		endPos.z += 75;
		Util::DrawBeam(startPos, endPos, 0, 255, 0);

		for (WaypointNode *connectedNode : *node->GetConnectedNodes())
			Util::DrawBeam(endPos, connectedNode->Pos, 255, 255, 255);
	}
}

static IPlayerInfo *_CheckCommandTargetPlayerExists() {
	edict_t *playerEdict = Engine->PEntityOfEntIndex(1);
	IPlayerInfo *playerInfo = IIPlayerInfoManager->GetPlayerInfo(playerEdict);
	if (!playerEdict || !playerInfo || !playerInfo->IsPlayer()) {
		Util::Log("No player found!");
		return nullptr;
	}
	return playerInfo;
}

CON_COMMAND(pongbot_createnode, "Creates a waypoint node wherever the first player is standing") {
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo) {
		uint8_t id = _WaypointNodes.size();
		if (id == 256) // Above max size of 8 bit (255)
			Util::Log("Max amount of waypoint nodes reached (255)!");
		else {
			_WaypointNodes.push_back(new WaypointNode(id, playerInfo->GetAbsOrigin()));
			Util::Log("Created waypoint node #%d", id);
		}
	}
}

CON_COMMAND(pongbot_connectnode1, "Selects nearest waypoint node for connection with another node") {
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo) {
		_SelectedNode = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
		if (!_SelectedNode)
			Util::Log("No waypoint node found!");
		else
			Util::Log("Waypoint node #%d selected", _SelectedNode->Id);
	}
}

CON_COMMAND(pongbot_connectnode2, "Connects previously selected waypoint node with nearest node") {
	if (!_SelectedNode)
		Util::Log("Select a node via pongbot_connectnode1 first");
	else {
		IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
		if (playerInfo) {
			WaypointNode *currentNode = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
			if (_SelectedNode == currentNode)
				Util::Log("Can't connect waypoint node to itself!");
			else {
				int selectedNodeID = _SelectedNode->Id;
				int currentNodeID = currentNode->Id;
				if (!_SelectedNode->ConnectToNode(currentNode, true)) // Connect bidirectional
					Util::Log("Node #%d and #%d were already connected!", selectedNodeID, currentNodeID);
				else {
					Util::Log("Connected waypoint node #%d with node #%d", selectedNodeID, currentNodeID);
					_SelectedNode = nullptr;
				}
			}
		}
	}
}

CON_COMMAND(pongbot_clearnodes, "Removes all waypoint nodes") {
	// Delete all nodes first to prevent memory leaks
	for (WaypointNode *node : _WaypointNodes)
		delete node;
	_WaypointNodes = vector<WaypointNode*>();
	Util::Log("All waypoint nodes cleared!");
}

CON_COMMAND(pongbot_removenode, "Removes the nearest node") {
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo) {
		WaypointNode *node = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
		if (!node)
			Util::Log("No waypoint node found!");
		else {
			// Delete from list first before deleting completely
			for (uint8_t i = 0; i < _WaypointNodes.size(); i++)
				if (_WaypointNodes[i] == node)
					_WaypointNodes.erase(_WaypointNodes.begin() + i);
			delete node;
			Util::Log("Removed nearest node!");
		}
	}
}