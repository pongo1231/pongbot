#pragma once
#include "ConVarBase.h"
#include "GameFramable.h"
#include <hlsdk/public/mathlib/mathlib.h>
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <vector>
#include <stack>

class WaypointNode;

class WaypointManager : public ConVarBase, public GameFramable {
public:
	static void Init();
	static void Destroy();
private:
	WaypointManager();
public:
	WaypointNode *GetRandomWaypointNode() const;
	WaypointNode *GetClosestWaypointNode(Vector pos) const;
	bool GetWaypointNodeStackToTargetNode(WaypointNode *startNode, WaypointNode *targetNode,
		std::stack<WaypointNode*> *waypointNodesStack,
		std::vector<WaypointNode*> *_alreadyTraversedWaypointNodesStack = nullptr);
public:
	void OnGameFrame();
};

extern WaypointManager *_WaypointManager;