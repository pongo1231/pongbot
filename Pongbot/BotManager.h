#pragma once
#include "ConVarBase.h"
#include "GameFramable.h"
#include <vector>

using namespace std;

class Bot;

class BotManager : public ConVarBase, public GameFramable {
public:
	static void Init();
	static void Destroy();
private:
	BotManager();
public:
	vector<Bot*> *GetAllBots();
	void KickBot(Bot *bot);
	void KickAllBots();
public:
	void OnGameFrame();
};

extern BotManager *_BotManager;