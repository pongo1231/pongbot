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
	uint16_t size = _WaypointNodes.size();
	if (size == 0)
		return nullptr;
	return _WaypointNodes[Util::RandomInt(0, size - 1)];
}

WaypointNode *WaypointManager::GetClosestWaypointNode(Vector pos) const {
	WaypointNode *closestNode = nullptr;
	float closestDistance = 9999999999999999; // Just something insanely high
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
	WaypointNode *targetNode, stack<WaypointNode*> *waypointNodeQueue) {
	if (startNode == targetNode)
		return true;
	else {
		bool isConnected = false;
		for (WaypointNode *node : *startNode->GetConnectedNodes())
			if (GetWaypointNodeQueueToTargetNode(node, targetNode, waypointNodeQueue)) {
				isConnected = true;
				waypointNodeQueue->push(targetNode);
			}
		return isConnected;
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

CON_COMMAND(pongbot_createwaypoint, "Creates a waypoint wherever the first player is standing") {
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo) {
		unsigned int id = _WaypointNodes.size();
		_WaypointNodes.push_back(new WaypointNode(id, playerInfo->GetAbsOrigin()));
		Util::Log("Created waypoint #%d", id);
	}
}

CON_COMMAND(pongbot_connectnode1, "Selects nearest node for connection with another node") {
	IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
	if (playerInfo) {
		_SelectedNode = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
		Util::Log("Node #%d selected", _SelectedNode->GetID());
	}
}

CON_COMMAND(pongbot_connectnode2, "Connects previously selected node with nearest node") {
	if (!_SelectedNode)
		Util::Log("Select a node via pongbot_connectnode1 first");
	else {
		IPlayerInfo *playerInfo = _CheckCommandTargetPlayerExists();
		if (playerInfo) {
			WaypointNode *currentNode = _WaypointManager->GetClosestWaypointNode(playerInfo->GetAbsOrigin());
			_SelectedNode->ConnectNode(currentNode);
			Util::Log("Connect node #%d with node #%d", _SelectedNode->GetID(), currentNode->GetID());
			_SelectedNode = nullptr;
		}
	}
}