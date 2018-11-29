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

static std::vector<Bot> _Bots;

SH_DECL_HOOK1_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool);

void BotManager::Init() {
	Assert(!_BotManager);
	_BotManager = new BotManager();
}

void BotManager::Destroy() {
	Assert(_BotManager);

	//SH_REMOVE_HOOK(IServerGameDLL, GameFrame, Server, SH_MEMBER(_BotManager, &BotManager::_OnGameFrame), true);
	delete _BotManager;
}

BotManager::BotManager() {
	g_pCVar = (ICvar*)((g_SMAPI->GetEngineFactory())(CVAR_INTERFACE_VERSION, nullptr));
	ConVar_Register(0, this);
	SH_ADD_HOOK(IServerGameDLL, GameFrame, Server, SH_MEMBER(this, &BotManager::_OnGameFrame), true);
}

bool BotManager::RegisterConCommandBase(ConCommandBase *cVar) {
	return META_REGCVAR(cVar);
}

bool BotManager::KickBot(edict_t *edict) {
	for (int i = _Bots.size() - 1; i > -1; i--) {
		edict_t *botEdict = _Bots[i].GetEdict();
		if (edict == botEdict) {
			int userId = Engine->GetPlayerUserId(botEdict);
			char command[64];
			sprintf_s(command, "kickid %d Bot kicked", userId);
			Engine->ServerCommand(command);
			_Bots.erase(_Bots.begin() + i);
			return true;
		}
	}
	return false;
}

void BotManager::_OnGameFrame(bool simulation) {
	if (simulation)
		for (int i = _Bots.size() - 1; i > -1; i--) {
			edict_t *botEdict = _Bots[i].GetEdict();
			if (!botEdict)
				KickBot(botEdict);
			else
				_Bots[i].Think();
		}
}

CON_COMMAND(pongbot_createbot, "Creates a new pongbot") {
	edict_t *botEdict = IIBotManager->CreateBot("Dummy");
	Assert(botEdict);

	_Bots.emplace_back(botEdict, "Dummy");
}