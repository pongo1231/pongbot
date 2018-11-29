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
	std::vector<Bot*> _Bots;
// Hooks
private:
	void _OnThink(bool running);
};

extern BotManager *_BotManager;