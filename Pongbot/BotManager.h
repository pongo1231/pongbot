#pragma once
#include "IConVarBase.h"
#include "IGameFramable.h"
#include <vector>

class Bot;

class BotManager : public IConVarBase, public IGameFramable
{
private:
	BotManager()
	{}

public:
	static void Init();
	static void Destroy();

	void KickBot(Bot *bot);
	void KickAllBots();
	void OnGameFrame();
};

extern BotManager *_BotManager;