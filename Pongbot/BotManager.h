#pragma once
#include "ConVarBase.h"
#include "GameFramable.h"
#include <vector>

class Bot;

class BotManager : public ConVarBase, public GameFramable
{
private:
	BotManager();

public:
	static void Init();
	static void Destroy();

	void KickBot(Bot *bot);
	void KickAllBots();
	void OnGameFrame();
};

extern BotManager *_BotManager;