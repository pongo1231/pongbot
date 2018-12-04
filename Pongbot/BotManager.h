#pragma once
#include "ConVarBase.h"
#include "GameFramable.h"
#include <vector>

class Bot;

class BotManager : public ConVarBase, public GameFramable {
public:
	static void Init();
	static void Destroy();
private:
	BotManager();
public:
	void KickBot(Bot *bot);
	void KickAllBots();
public:
	void OnGameFrame();
};

extern BotManager *_BotManager;