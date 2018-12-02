#pragma once
#include "WaypointNode.h"
#include "ConVarBase.h"
#include <hlsdk/public/mathlib/mathlib.h>
#include <vector>

using namespace std;

class WaypointManager : public ConVarBase {
public:
	static void Init();
	static void Destroy();
private:
	WaypointManager();
public:
	WaypointNode *GetRandomWaypointNode() const;
	WaypointNode *GetClosestWaypointNode(Vector pos) const;
};

extern WaypointManager *_WaypointManager;