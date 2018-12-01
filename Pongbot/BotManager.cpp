#include "BotManager.h"
#include <metamod/ISmmPlugin.h>
#include <metamod/sourcehook.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <iostream>

extern IVEngineServer *Engine;
extern IBotManager *IIBotManager;
extern IServerGameDLL *Server;
extern ISmmAPI *g_SMAPI;
extern ISmmPlugin *g_PLAPI;
extern SourceHook::ISourceHook *g_SHPtr;
extern PluginId g_PLID;

BotManager *_BotManager;

static vector<Bot*> _Bots;

SH_DECL_HOOK1_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool);

void BotManager::Init() {
	Assert(!_BotManager);
	_BotManager = new BotManager();

	g_pCVar = (ICvar*)((g_SMAPI->GetEngineFactory())(CVAR_INTERFACE_VERSION, nullptr));
	ConVar_Register(0, _BotManager);
	SH_ADD_HOOK(IServerGameDLL, GameFrame, Server, SH_MEMBER(_BotManager, &BotManager::_OnGameFrame), true);
}

void BotManager::Destroy() {
	Assert(_BotManager);
	SH_REMOVE_HOOK(IServerGameDLL, GameFrame, Server, SH_MEMBER(_BotManager, &BotManager::_OnGameFrame), true);
	_BotManager->KickAllBots();

	delete _BotManager;
}

BotManager::BotManager() {}

bool BotManager::RegisterConCommandBase(ConCommandBase *cVar) {
	return META_REGCVAR(cVar);
}

vector<Bot*> *BotManager::GetAllBots() {
	return &_Bots;
}

void BotManager::KickBot(Bot *bot) {
	int userId = Engine->GetPlayerUserId(bot->GetEdict());
	char command[64];
	sprintf_s(command, "kickid %d Bot Removed\n", userId);
	Engine->ServerCommand(command);
}

void BotManager::KickAllBots() {
	for (Bot *bot : _Bots)
		KickBot(bot);
}

void BotManager::_OnGameFrame(bool simulation) {
	for (uint8_t i = 0; i < _Bots.size(); i++) {
		if (!_Bots[i]->Exists()) {
			delete _Bots[i];
			_Bots.erase(_Bots.begin() + i);
		} else
			_Bots[i]->Think();
	}
}

CON_COMMAND(pongbot_createbot, "Creates a new bot") {
	edict_t *botEdict = IIBotManager->CreateBot("Dummy");
	Assert(botEdict);
	_Bots.push_back(new Bot(botEdict, "Dummy"));
};

CON_COMMAND(pongbot_kickbots, "Kicks all bots") {
	_BotManager->KickAllBots();
};