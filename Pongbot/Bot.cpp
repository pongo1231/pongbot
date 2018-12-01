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
const edict_t *_Edict;
IBotController *_IIBotController;
IPlayerInfo *_IIPlayerInfo;
BotHelper *_BotHelper;
TFClass _CurrentClass;

Bot::Bot(edict_t *edict, const char *name) : Name(name), _Edict(edict),
	_IIBotController(IIBotManager->GetBotController(edict)),
	_IIPlayerInfo(IIPlayerInfoManager->GetPlayerInfo(edict)),
	_BotHelper(new BotHelper(this)) {
	_IIPlayerInfo->ChangeTeam(2);
	_RandomClass();
}

Bot::~Bot() {
	delete _BotHelper;
}

const edict_t *Bot::GetEdict() const {
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

void Bot::Think() {
	std::vector<Bot*> *bots = _BotManager->GetAllBots();
	Vector targetPos;
	Bot *targetBot = bots->at(0);
	if (targetBot) {
		CBotCmd cmd;
		cmd.viewangles = _BotHelper->GetLookAtAngle(targetBot->GetOrigin());

		vec_t forth, side;
		_BotHelper->GetIdealMoveSpeeds(targetBot->GetOrigin(), forth, side);
		cmd.forwardmove = forth;
		cmd.sidemove = side;

		_IIBotController->RunPlayerMove(&cmd);
	}
}

void Bot::_ChangeClass(TFClass tfClass) {
	char newClass[16];
	switch(tfClass) {
		case SCOUT:
			strcpy(newClass, "scout");
			break;
		case SOLDIER:
			strcpy(newClass, "soldier");
			break;
		case PYRO:
			strcpy(newClass, "pyro");
			break;
		case DEMO:
			strcpy(newClass, "demoman");
			break;
		case HEAVY:
			strcpy(newClass, "heavyweapons");
			break;
		case ENGI:
			strcpy(newClass, "engineer");
			break;
		case MED:
			strcpy(newClass, "medic");
			break;
		case SNIPER:
			strcpy(newClass, "sniper");
			break;
		case SPY:
			strcpy(newClass, "spy");
			break;
	}
	char cmd[32];
	sprintf_s(cmd, "joinclass %s", newClass);
	IIServerPluginHelpers->ClientCommand((edict_t*) _Edict, cmd);
	_CurrentClass = tfClass;
}

void Bot::_RandomClass() {
	_ChangeClass(TFClass(Util::RandomInt(0, 8)));
}