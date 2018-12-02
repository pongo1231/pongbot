#pragma once
#include "Bot.h"
#include "ConVarBase.h"
#include <vector>

using namespace std;

class BotManager : public ConVarBase {
public:
	static void Init();
	static void Destroy();
private:
	BotManager();
public:
	vector<Bot*> *GetAllBots();
	void KickBot(Bot *bot);
	void KickAllBots();
// Hooks
private:
	void _OnGameFrame(bool simulation);
};

extern BotManager *_BotManager;