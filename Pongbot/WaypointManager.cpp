#include "WaypointManager.h"
#include "Util.h"
#include <metamod/ISmmPlugin.h>
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

extern IVEngineServer *Engine;
extern IPlayerInfoManager *IIPlayerInfoManager;

WaypointManager *_WaypointManager;

static vector<WaypointNode*> _WaypointNodes;

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
	WaypointNode *closestNode;
	float closestDistance;
	for (WaypointNode *waypointNode : _WaypointNodes) {
		float distance = closestDistance > waypointNode->Pos.DistTo(pos);
		if (!closestDistance || closestDistance > distance) {
			closestNode = waypointNode;
			closestDistance = distance;
		}
	}
	return closestNode;
}

CON_COMMAND(pongbot_createwaypoint, "Creates a waypoint wherever the first player is standing") {
	edict_t *playerEdict = Engine->PEntityOfEntIndex(1);
	IPlayerInfo *playerInfo = IIPlayerInfoManager->GetPlayerInfo(playerEdict);
	if (!playerEdict || !playerInfo || !playerInfo->IsPlayer())
		Util::Log("No player found!");
	else {
		unsigned int id = _WaypointNodes.size();
		_WaypointNodes.push_back(new WaypointNode(id, playerInfo->GetAbsOrigin()));
		Util::Log("Created waypoint #%d", id);
	}
}