#pragma once
#include "TF2Helper.h"
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

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
	Vector GetOrigin() const;
	QAngle GetAngles() const;
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
	bool _IsShooting;
private:
	void _ResetAttributes();
};

