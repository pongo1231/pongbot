#pragma once
#include "ConVarBase.h"
#include "GameFramable.h"
#include <hlsdk/public/mathlib/mathlib.h>
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <vector>
#include <stack>

using namespace std;

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
	bool GetWaypointNodeQueueToTargetNode(WaypointNode *startNode, WaypointNode *targetNode,
		stack<WaypointNode*> *waypointNodesStack,
		vector<WaypointNode*> *_alreadyTraversedWaypointNodesStack = nullptr);
public:
	void OnGameFrame();
};

extern WaypointManager *_WaypointManager;