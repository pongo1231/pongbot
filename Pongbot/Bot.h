#pragma once
#include "TF2Helper.h"
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

class BotHelper;

class Bot {
public:
	Bot(edict_t *edict, const char *name);
	~Bot();
	const char *Name;
	const edict_t *GetEdict() const;
	bool Exists() const;
	Vector GetOrigin() const;
	QAngle GetAngles() const;
	TFClass GetClass() const;
	void Think();
private:
	const edict_t *_Edict;
	IBotController *_IIBotController;
	IPlayerInfo *_IIPlayerInfo;
	BotHelper *_BotHelper;
	TFClass _CurrentClass;
private:
	void _ChangeClass(TFClass tfClass);
	void _RandomClass();
};

