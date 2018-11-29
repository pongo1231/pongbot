#include "BotManager.h"
#include <metamod/ISmmPlugin.h>
#include <metamod/sourcehook.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <string>

extern IVEngineServer *Engine;
extern IBotManager *IIBotManager;
extern IServerGameDLL *Server;
extern ISmmAPI *g_SMAPI;
extern ISmmPlugin *g_PLAPI;
extern SourceHook::ISourceHook *g_SHPtr;
extern PluginId g_PLID;

BotManager *_BotManager;

std::vector<Bot*> _Bots;

SH_DECL_HOOK1_void(IServerGameDLL, Think, SH_NOATTRIB, 0, bool);

void BotManager::Init() {
	Assert(!_BotManager);

	_BotManager = new BotManager();
	SH_ADD_HOOK(IServerGameDLL, Think, Server, SH_MEMBER(_BotManager, &BotManager::_OnThink), true);
}

void BotManager::Destroy() {
	Assert(_BotManager);

	SH_REMOVE_HOOK(IServerGameDLL, Think, Server, SH_MEMBER(_BotManager, &BotManager::_OnThink), true);
	delete _BotManager;
}

BotManager::BotManager() {
	g_pCVar = (ICvar*)((g_SMAPI->GetEngineFactory())(CVAR_INTERFACE_VERSION, nullptr));
	ConVar_Register(0, this);
}

bool BotManager::RegisterConCommandBase(ConCommandBase *cVar) {
	return META_REGCVAR(cVar);
}

bool BotManager::KickBot(edict_t *edict) {
	for (int i = _Bots.size() - 1; i > -1; i--) {
		edict_t *botEdict = _Bots[i]->GetEdict();
		if (edict == botEdict) {
			int userId = Engine->GetPlayerUserId(botEdict);
			char command[64];
			sprintf_s(command, "kickid %d Bot kicked", userId);
			Engine->ServerCommand(command);
			delete _Bots[i];
			_Bots.erase(_Bots.begin() + i);
			return true;
		}
	}
	return false;
}

void BotManager::_OnThink(bool running) {
	if (running)
		for (Bot *bot : _Bots) {
			edict_t *botEdict = bot->GetEdict();
			if (!botEdict)
				KickBot(botEdict);
			else
				bot->Think();
		}
}

CON_COMMAND(pongbot_createbot, "Creates a new pongbot") {
	edict_t *botEdict = IIBotManager->CreateBot("Dummy");
	Assert(botEdict);

	_Bots.push_back(new Bot(botEdict, "Dummy"));
}