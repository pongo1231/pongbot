#include "WaypointManager.h"
#include "Util.h"
#include <metamod/ISmmPlugin.h>

extern IVEngineServer *Engine;
extern IPlayerInfoManager *IIPlayerInfoManager;

WaypointManager *_WaypointManager;

static vector<WaypointNode*> _WaypointNodes;
static WaypointNode *_SelectedNode;

void WaypointManager::Init() {
	Assert(!_WaypointManager);
	_WaypointManager = new WaypointManager();
}

void WaypointManager::Destroy() {
	Assert(_WaypointManager);
	for (uint16_t i = 0; i < _WaypointNodes.size(); i++) {
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

bool WaypointManager::GetWaypointNodeQueueToTargetNode(WaypointNode *startNode,
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
			if (GetWaypointNodeQueueToTargetNode(node, targetNode, waypointNodesStack,
				_alreadyTraversedWaypointNodesStack)) {
				waypointNodesStack->push(startNode);
				return true;
			}
	}
	return false;
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
		unsigned int id = _WaypointNodes.size();
		_WaypointNodes.push_back(new WaypointNode(id, playerInfo->GetAbsOrigin()));
		Util::Log("Created waypoint node #%d", id);
	}
}

CON_COMMAND(pongbot_connectnode1, "Selects nearest waypoint node for connection with another node") {
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo) {
		_SelectedNode = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
		Util::Log("Waypoint node #%d selected", _SelectedNode->GetID());
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
				int selectedNodeID = _SelectedNode->GetID();
				int currentNodeID = currentNode->GetID();
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