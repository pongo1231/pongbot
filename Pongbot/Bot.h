#pragma once
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

class Bot {
public:
	Bot(edict_t *edict, const char *name);
	const char *Name;
	edict_t *GetEdict();
	void Think();
private:
	edict_t *_Edict;
	IBotController *_IIBotController;
	IPlayerInfo *_IIPlayerInfo;
};

