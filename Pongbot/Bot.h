#pragma once
#include "TF2Helper.h"
#include "WaypointNode.h"
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <cstdint>
#include <stack>

class BotHelper;

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
	uint8_t _PosStuckTime;
	stack<WaypointNode*> _WaypointNodeStack;
// Button states
private:
	bool _IsShooting;
	bool _IsJumping;
	bool _IsCrouching;
private:
	void _ResetStates();
	int _ConstructButtonsState();
	void _UpdateNewTargetWaypointNode();
};

