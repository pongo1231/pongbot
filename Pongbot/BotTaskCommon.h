#pragma once
#include "BotTask.h"
#include <stack>

class WaypointNode;

class BotTaskCommon : public BotTask {
public:
	BotTaskCommon(Bot *bot);

public:
	void OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt, int *&taskFlags);

private:
	Vector _LastPos;
	unsigned int _PosStuckTime;
	std::stack<WaypointNode*> _WaypointNodeStack;
	WaypointNode *_ClosestWaypointNode;

	void _UpdateNewWaypointNodeStack();
	void _UpdateClosestWaypointNode();
};