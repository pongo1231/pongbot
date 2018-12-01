#pragma once
#include "Bot.h"
#include <hlsdk/public/tier1/convar.h>
#include <vector>

using namespace std;

class BotManager : public IConCommandBaseAccessor {
public:
	static void Init();
	static void Destroy();
public:
	vector<Bot*> *GetAllBots();
	void KickBot(Bot *bot);
	void KickAllBots();
private:
	BotManager();
// Hooks
private:
	bool RegisterConCommandBase(ConCommandBase *pVar);
	void _OnGameFrame(bool simulation);
};

extern BotManager *_BotManager;