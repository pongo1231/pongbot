#pragma once
#include "IConVarBase.h"
#include "IGameFramable.h"
#include <hlsdk/public/mathlib/mathlib.h>
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <vector>
#include <stack>

class WaypointNode;

class WaypointManager : public IConVarBase, public IGameFramable
{
private:
	WaypointManager();

public:
	static void Init();
	static void Destroy();

	WaypointNode *GetRandomWaypointNode() const;
	WaypointNode *GetClosestWaypointNode(Vector pos) const;
	vec_t GetShortestWaypointNodeRouteToTargetNode(WaypointNode *startNode, WaypointNode *targetNode,
		std::stack<WaypointNode*> *waypointNodesStack,
		std::vector<WaypointNode*> *_alreadyTraversedWaypointNodesStack = nullptr);
	void OnGameFrame();
};

extern WaypointManager *_WaypointManager;