#include <stdafx.h>
#include "BotManager.h"
#include "../Util.h"
#include "Bot.h"
#include "Visibles/BotVisiblesProvider.h"
#include "../TF2/Entity/Player.h"
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <iostream>

extern IVEngineServer* Engine;
extern IBotManager* IIBotManager;

BotManager* _BotManager = nullptr;

static std::vector<Bot*> _Bots;

void BotManager::Init()
{
	if (!_BotManager)
	{
		Util::DebugLog("INIT BotManager");
		_Bots.clear();
		BotVisiblesProvider::Init();

		_BotManager = new BotManager();
	}
}

void BotManager::Destroy()
{
	if (_BotManager)
	{
		Util::DebugLog("DESTROY BotManager");
		BotVisiblesProvider::Destroy();

		_BotManager->KickAllBots();
		delete _BotManager;
		_BotManager = nullptr;
	}
}

void BotManager::KickBot(Bot* bot)
{
	char command[64];
	snprintf(command, sizeof(command), "kickid %d Bot Removed\n", Engine->GetPlayerUserId(bot->GetEdict()));
	Engine->ServerCommand(command);
}

void BotManager::KickAllBots()
{
	for (Bot* bot : _Bots)
	{
		KickBot(bot);
	}
}

bool BotManager::BotsInGame() const
{
	return !_Bots.empty();
}

void BotManager::OnGameFrame()
{
	for (uint8_t i = 0; i < _Bots.size(); i++)
	{
		Bot* bot = _Bots[i];
		if (!bot->Exists())
		{
			Util::DebugLog("Removed Bot %s (Edict Index: %d)", bot->Name, bot->GetEdict()->m_iIndex);
			delete bot;
			_Bots.erase(_Bots.begin() + i);
		}
		else
		{
			bot->Think();
		}
	}
}

CON_COMMAND(pongbot_bot_add, "Adds a new bot")
{
	Player botPlayer(IIBotManager->CreateBot("Pongbot"));
	if (!botPlayer.Exists())
	{
		Util::Log("Error while creating bot!");
	}
	else
	{
		Bot* bot = new Bot(botPlayer, "Pongbot");
		_Bots.push_back(bot);
		Util::Log("Created Bot %s (Edict Index: %d)", bot->Name, bot->GetEdict()->m_iIndex);
	}
};

CON_COMMAND(pongbot_bot_kickall, "Kicks all bots")
{
	_BotManager->KickAllBots();
	Util::Log("Kicked all bots");
};