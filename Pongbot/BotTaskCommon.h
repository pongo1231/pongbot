#pragma once
#include "BotTask.h"
#include <stack>

class WaypointNode;

class BotTaskCommon : public BotTask
{
public:
	BotTaskCommon(Bot *bot);

public:
	void OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt);

private:
	Vector _LastPos;
	unsigned int _PosStuckTime;
	std::stack<WaypointNode*> _WaypointNodeStack;
	/* CTF */
	bool _DoCTFBehaviour;
	edict_t* _AllyFlag;
	edict_t* _EnemyFlag;
	CTFFlagStatusType _PrevFlagStatus;

	void _DoMovement(int *&pressedButtons, Vector2D *&movement);
	void _DoLooking(int *&pressedButtons, QAngle *&lookAt);
	void _UpdateNewWaypointNodeStack();
	void _UpdateObjectives();
};