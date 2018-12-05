#pragma once
#include "BotTask.h"
#include <stack>

class WaypointNode;

class BotTaskRoamAround : public BotTask {
public:
	BotTaskRoamAround(Bot *bot);

	void OnTick(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt);

private:
	Vector _LastPos;
	unsigned int _PosStuckTime;
	std::stack<WaypointNode*> _WaypointNodeStack;
	WaypointNode *_ClosestWaypointNode;

	void _UpdateNewWaypointNodeStack();
	void _UpdateClosestWaypointNode();
};