#include "Bot.h"
#include "BotManager.h"
#include "Util.h"
#include "BotHelper.h"
#include <metamod/ISmmPlugin.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <hlsdk/game/shared/in_buttons.h>
#include <string>

using namespace std;

extern IVEngineServer *Engine;
extern IBotManager *IIBotManager;
extern IPlayerInfoManager *IIPlayerInfoManager;
extern IServerPluginHelpers *IIServerPluginHelpers;

const char *Name;
edict_t *_Edict; // Not const for convenience reasons
IBotController *_IIBotController;
IPlayerInfo *_IIPlayerInfo;
BotHelper *_BotHelper;

TFClass _CurrentClass;
bool _IsShooting;

Bot::Bot(edict_t *edict, const char *name) : Name(name), _Edict(edict),
	_IIBotController(IIBotManager->GetBotController(edict)),
	_IIPlayerInfo(IIPlayerInfoManager->GetPlayerInfo(edict)),
	_BotHelper(new BotHelper(this)) {
	_IIPlayerInfo->ChangeTeam(2);
	_ResetAttributes();
	_BotHelper->RandomClass();
}

Bot::~Bot() {
	delete _BotHelper;
}

void Bot::Think() {
	std::vector<Bot*> *bots = _BotManager->GetAllBots();
	Vector targetPos;
	Bot *targetBot = bots->at(0);
	if (targetBot)
		_IIBotController->RunPlayerMove(&_BotHelper->ConstructBotCmd(
			_BotHelper->GetLookAtAngle(targetBot->GetOrigin()),
			_BotHelper->GetIdealMoveSpeeds(targetBot->GetOrigin()),
			_IsShooting
			));
}

edict_t *Bot::GetEdict() const {
	return _Edict;
}

bool Bot::Exists() const {
	return _IIPlayerInfo->IsConnected();
}

Vector Bot::GetOrigin() const {
	return _IIPlayerInfo->GetAbsOrigin();
}

QAngle Bot::GetAngles() const {
	return _IIPlayerInfo->GetAbsAngles();
}

TFClass Bot::GetClass() const {
	return _CurrentClass;
}

void Bot::_ResetAttributes() {
	_IsShooting = false;
}

void Bot::ChangeClass(TFClass tfClass) {
	char newClass[16];
	_BotHelper->TFClassToJoinName(tfClass, newClass);
	char cmd[32];
	sprintf_s(cmd, "joinclass %s", newClass);
	IIServerPluginHelpers->ClientCommand(_Edict, cmd);
	_CurrentClass = tfClass;
}