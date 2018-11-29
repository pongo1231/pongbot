#pragma once
#include "Bot.h"
#include <hlsdk/public/tier1/convar.h>
#include <vector>

class BotManager : public IConCommandBaseAccessor {
public:
	static void Init();
	static void Destroy();
	bool RegisterConCommandBase(ConCommandBase *pVar);
	bool KickBot(edict_t *edict);
private:
	BotManager();
// Hooks
private:
	void _OnGameFrame(bool simulation);
};

extern BotManager *_BotManager;