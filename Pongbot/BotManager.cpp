#include "BotManager.h"
#include "Util.h"
#include "Bot.h"
#include "BotVisiblesProvider.h"
#include <metamod/ISmmAPI.h>
#include <metamod/sourcehook.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <iostream>

extern IVEngineServer *Engine;
extern IBotManager *IIBotManager;

BotManager *_BotManager;

static std::vector<Bot*> _Bots;

void BotManager::Init()
{
	Assert(!_BotManager);
	_Bots.clear();
	BotVisiblesProvider::Init();

	_BotManager = new BotManager();
}

void BotManager::Destroy()
{
	Assert(_BotManager);
	BotVisiblesProvider::Destroy();

	_BotManager->KickAllBots();
	delete _BotManager;
}

void BotManager::KickBot(Bot *bot)
{
	char command[64];
	sprintf_s(command, "kickid %d Bot Removed\n", Engine->GetPlayerUserId(bot->GetEdict()));
	Engine->ServerCommand(command);
}

void BotManager::KickAllBots()
{
	for (Bot *bot : _Bots)
		KickBot(bot);
}

bool BotManager::BotsInGame() const
{
	return !_Bots.empty();
}

void BotManager::OnGameFrame()
{
	for (uint8_t i = 0; i < _Bots.size(); i++)
	{
		Bot *bot = _Bots[i];
		if (!bot->Exists())
		{
			delete bot;
			_Bots.erase(_Bots.begin() + i);
		}
		else
			bot->Think();
	}
}

CON_COMMAND(pongbot_bot_add, "Adds a new bot")
{
	edict_t *botEdict = IIBotManager->CreateBot("Pongbot");
	if (!botEdict)
		Util::Log("Error while creating bot!");
	else
		_Bots.push_back(new Bot(botEdict, "Pongbot"));
};

CON_COMMAND(pongbot_bot_kickall, "Kicks all bots")
{
	_BotManager->KickAllBots();
};