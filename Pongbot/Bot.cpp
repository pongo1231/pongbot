#include "Bot.h"
#include "BotManager.h"
#include "Util.h"
#include "BotTaskMaster.h"
#include "BotVisibles.h"
#include <metamod/ISmmPlugin.h>
#include <string>

extern IVEngineServer *Engine;
extern IBotManager *IIBotManager;
extern IPlayerInfoManager *IIPlayerInfoManager;
extern IServerPluginHelpers *IIServerPluginHelpers;
extern IServerGameClients *IIServerGameClients;

const char *Name;
edict_t *_Edict;
IBotController *_IIBotController;
IPlayerInfo *_IIPlayerInfo;
BotTaskMaster *_BotTaskMaster;
BotVisibles *_BotVisibles;

TFClass _CurrentClass;

Bot::Bot(edict_t *edict, const char *name) : Name(name), _Edict(edict),
	_IIBotController(IIBotManager->GetBotController(edict)),
	_IIPlayerInfo(IIPlayerInfoManager->GetPlayerInfo(edict)),
	_BotVisibles(new BotVisibles(this)) {
	_IIPlayerInfo->ChangeTeam(2);
	_RandomClass();
}

Bot::~Bot() {
	delete _BotTaskMaster;
	delete _BotVisibles;
}

void Bot::Think() {
	int pressedButtons = 0;
	Vector2D *movement = nullptr;
	QAngle *lookAt = nullptr;
	_BotTaskMaster->OnThink(&pressedButtons, movement, lookAt);
	if (!movement)
		movement = new Vector2D();
	if (!lookAt)
		lookAt = new QAngle();

	CBotCmd cmd;
	cmd.buttons = pressedButtons;
	cmd.forwardmove = movement->x;
	cmd.sidemove = movement->y;
	cmd.viewangles = *lookAt;
	_IIBotController->RunPlayerMove(&cmd);

	delete movement;
	delete lookAt;
}

edict_t *Bot::GetEdict() const {
	return _Edict;
}

bool Bot::Exists() const {
	return _IIPlayerInfo->IsConnected();
}

Vector Bot::GetPos() const {
	return _IIPlayerInfo->GetAbsOrigin();
}

Vector Bot::GetEarPos() const {
	Vector earPos;
	IIServerGameClients->ClientEarPosition(_Edict, &earPos);
	return earPos;
}

QAngle Bot::GetAngle() const {
	return _IIPlayerInfo->GetAbsAngles();
}

TFClass Bot::GetClass() const {
	return _CurrentClass;
}

BotVisibles *Bot::GetBotVisibles() const {
	return _BotVisibles;
}

void Bot::ChangeClass(TFClass tfClass) {
	char newClass[16];
	_TFClassToJoinName(tfClass, newClass);
	char cmd[32];
	sprintf_s(cmd, "joinclass %s", newClass);
	IIServerPluginHelpers->ClientCommand(_Edict, cmd);
	_CurrentClass = tfClass;
}

void Bot::_TFClassToJoinName(TFClass tfClass, char *tfClassName) {
	switch (tfClass) {
	case SCOUT:
		strcpy(tfClassName, "scout");
		break;
	case SOLDIER:
		strcpy(tfClassName, "soldier");
		break;
	case PYRO:
		strcpy(tfClassName, "pyro");
		break;
	case DEMO:
		strcpy(tfClassName, "demoman");
		break;
	case HEAVY:
		strcpy(tfClassName, "heavyweapons");
		break;
	case ENGI:
		strcpy(tfClassName, "engineer");
		break;
	case MED:
		strcpy(tfClassName, "medic");
		break;
	case SNIPER:
		strcpy(tfClassName, "sniper");
		break;
	case SPY:
		strcpy(tfClassName, "spy");
		break;
	}
}

void Bot::_RandomClass() {
	ChangeClass(TFClass(Util::RandomInt(0, 8)));
}