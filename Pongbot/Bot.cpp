#include "Bot.h"
#include "BotManager.h"
#include <metamod/ISmmPlugin.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

extern IVEngineServer *Engine;
extern IBotManager *IIBotManager;
extern IPlayerInfoManager *IIPlayerInfoManager;
extern IServerPluginHelpers *IIServerPluginHelpers;

const char *Name;
edict_t *_Edict;
IBotController *_IIBotController;
IPlayerInfo *_IIPlayerInfo;

Bot::Bot(edict_t *edict, const char *name) : Name(name), _Edict(edict) {
	_IIBotController = IIBotManager->GetBotController(edict);
	_IIPlayerInfo = IIPlayerInfoManager->GetPlayerInfo(edict);

	_IIPlayerInfo->ChangeTeam(2);
	IIServerPluginHelpers->ClientCommand(edict, "joinclass pyro");
}

edict_t *Bot::GetEdict() {
	return _Edict;
}

void Bot::Think() {
	CBotCmd cmd;
	cmd.forwardmove = 40.0;
	if (++cmd.mousedx == 255)
		cmd.mousedx = 0;
	cmd.upmove = 5.0;
	_IIBotController->RunPlayerMove(&cmd);
}