#pragma once
#include "TF2Helper.h"
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <vector>
#include <stack>

using namespace std;

class BotHelper;
class WaypointNode;

class Bot {
public:
	const char *Name;
public:
	Bot(edict_t *edict, const char *name);
	~Bot();
public:
	void Think();
	edict_t *GetEdict() const;
	bool Exists() const;
	Vector GetPos() const;
	Vector GetEarPos() const;
	QAngle GetAngle() const;
	TFClass GetClass() const;
public:
	void ChangeClass(TFClass tfClass);
private:
	edict_t *_Edict;
	IBotController *_IIBotController;
	IPlayerInfo *_IIPlayerInfo;
	BotHelper *_BotHelper;
private:
	TFClass _CurrentClass;
	Vector _LastPos;
	unsigned int _PosStuckTime;
	stack<WaypointNode*> _WaypointNodeStack;
	int _PressedButtons;
	WaypointNode *_ClosestWaypointNode;
private:
	void _ResetState();
	void _UpdateNewWaypointNodeStack();
	void _UpdateClosestWaypointNode();
};

