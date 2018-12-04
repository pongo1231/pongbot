#include "BotManager.h"
#include "Util.h"
#include "Bot.h"
#include <metamod/ISmmAPI.h>
#include <metamod/sourcehook.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <iostream>

extern IVEngineServer *Engine;
extern IBotManager *IIBotManager;

BotManager *_BotManager;

static std::vector<Bot*> _Bots;

void BotManager::Init() {
	Assert(!_BotManager);
	_BotManager = new BotManager();
}

void BotManager::Destroy() {
	Assert(_BotManager);
	_BotManager->KickAllBots();

	delete _BotManager;
}

BotManager::BotManager() {}

void BotManager::KickBot(Bot *bot) {
	char command[64];
	sprintf_s(command, "kickid %d Bot Removed\n", Engine->GetPlayerUserId(bot->GetEdict()));
	Engine->ServerCommand(command);
}

void BotManager::KickAllBots() {
	for (Bot *bot : _Bots)
		KickBot(bot);
}

void BotManager::OnGameFrame() {
	for (uint8_t i = 0; i < _Bots.size(); i++) {
		Bot *bot = _Bots[i];
		if (!bot->Exists()) {
			delete bot;
			_Bots.erase(_Bots.begin() + i);
		}
		else
			bot->Think();
	}
}

CON_COMMAND(pongbot_createbot, "Creates a new bot") {
	edict_t *botEdict = IIBotManager->CreateBot("Dummy");
	if (!botEdict)
		Util::Log("Error while creating bot!");
	else
		_Bots.push_back(new Bot(botEdict, "Dummy"));
};

CON_COMMAND(pongbot_kickbots, "Kicks all bots") {
	_BotManager->KickAllBots();
};