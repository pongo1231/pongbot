#include "Bot.h"
#include "BotManager.h"
#include "Util.h"
#include "BotBrain.h"
#include "BotVisibles.h"
#include "BotBrainScout.h"
#include "BotBrainSoldier.h"
#include "BotBrainPyro.h"
#include "BotBrainDemo.h"
#include "BotBrainHeavy.h"
#include "BotBrainEngi.h"
#include "BotBrainMed.h"
#include "BotBrainSniper.h"
#include "BotBrainSpy.h"
#include "TFTeam.h"
#include "TFClass.h"
#include "EntityDataProvider.h"
#include "Util.h"
#include <metamod/ISmmPlugin.h>
#include <string>

ConVar _CVarAimSensivity("pongbot_bot_aimsensivity", "3.0", 0, "Bot aim sensivity");

extern IVEngineServer *Engine;
extern IBotManager *IIBotManager;
extern IPlayerInfoManager *IIPlayerInfoManager;
extern IServerPluginHelpers *IIServerPluginHelpers;
extern IServerGameClients *IIServerGameClients;

const char *Name;
edict_t *_Edict;
IBotController *_IIBotController;
IPlayerInfo *_IIPlayerInfo;
BotBrain *_BotBrain;
BotVisibles *_BotVisibles;
TFClass _CurrentClass;
QAngle _TargetViewAngle;
Vector2D _Movement;
int _PressedButtons;
WeaponSlot _SelectedWeapon;

Bot::Bot(edict_t *edict, const char *name) : Name(name), _Edict(edict),
	_IIBotController(IIBotManager->GetBotController(edict)),
	_IIPlayerInfo(IIPlayerInfoManager->GetPlayerInfo(edict))
{
	_BotBrain = new BotBrain(this);
	_BotVisibles = new BotVisibles(this);

	_SwitchToFittingTeam();
	_RandomClass();
}

Bot::~Bot()
{
	delete _BotVisibles;
	delete _BotBrain;
}

void Bot::Think()
{
	_BotVisibles->OnThink();
	_BotBrain->OnThink();
	
	// Smoothed out aiming
	QAngle currentViewAngle = GetViewAngle();
	QAngle finalViewAngle = Util::CorrectViewAngle(_TargetViewAngle - currentViewAngle) / _CVarAimSensivity.GetFloat() + currentViewAngle;
	finalViewAngle.x *= 2;

	CBotCmd cmd;
	cmd.buttons = _PressedButtons;
	cmd.forwardmove = _Movement.x;
	cmd.sidemove = _Movement.y;
	cmd.viewangles = finalViewAngle;
	cmd.weaponselect = _SelectedWeapon;
	_IIBotController->RunPlayerMove(&cmd);
}

edict_t *Bot::GetEdict() const
{
	return _Edict;
}

bool Bot::Exists() const
{
	return _IIPlayerInfo->IsConnected();
}

Vector Bot::GetPos() const
{
	return _IIBotController->GetLocalOrigin();
}

Vector Bot::GetEarPos() const
{
	Vector earPos;
	IIServerGameClients->ClientEarPosition(_Edict, &earPos);
	return earPos;
}

QAngle Bot::GetViewAngle() const
{
	return _IIBotController->GetLocalAngles();
}

void Bot::SetViewAngle(QAngle angle)
{
	_TargetViewAngle = angle;
}

TFClass Bot::GetClass() const
{
	return _CurrentClass;
}

TFTeam Bot::GetTeam() const
{
	return (TFTeam) _IIPlayerInfo->GetTeamIndex();
}

BotVisibles *Bot::GetBotVisibles() const
{
	return _BotVisibles;
}

void Bot::SetMovement(Vector2D movement)
{
	_Movement = movement;
}

void Bot::SetPressedButtons(int pressedButtons)
{
	_PressedButtons = pressedButtons;
}

void Bot::SetSelectedWeapon(WeaponSlot weapon)
{
	//_SelectedWeapon = weapon;
	ExecClientCommand("slot%d", weapon);
}

bool Bot::IsDead() const
{
	return _IIPlayerInfo->IsDead();
}

void Bot::ChangeClass(TFClass tfClass)
{
	char newClass[16];
	_TFClassToJoinName(tfClass, newClass);

	char cmd[32];
	sprintf(cmd, "joinclass %s", newClass);
	IIServerPluginHelpers->ClientCommand(_Edict, cmd);

	_CurrentClass = tfClass;
	_UpdateBotBrain();
}

void Bot::ExecClientCommand(const char *command, ...) const
{
	char fullcommand[128];
	va_list args;
	va_start(args, fullcommand);
	vsnprintf(fullcommand, sizeof(fullcommand), command, args);
	va_end(args);

	IIServerPluginHelpers->ClientCommand(_Edict, fullcommand);
}

void Bot::_SwitchToFittingTeam()
{
	uint8_t red = 0;
	uint8_t blue = 0;
	for (edict_t *edict : Util::GetAllPlayers())
	{
		TFTeam team = _EntityDataProvider->GetDataFromEdict<TFTeam>(edict, DATA_TEAM);
		if (team == TEAM_RED)
			red++;
		else if (team == TEAM_BLUE)
			blue++;
	}

	_IIPlayerInfo->ChangeTeam(blue < red ? TEAM_BLUE : TEAM_RED);
}

void Bot::_UpdateBotBrain()
{
	delete _BotBrain;
	switch (_CurrentClass)
	{
	case SCOUT:
		_BotBrain = new BotBrainScout(this);
	case SOLDIER:
		_BotBrain = new BotBrainSoldier(this);
	case PYRO:
		_BotBrain = new BotBrainPyro(this);
	case DEMO:
		_BotBrain = new BotBrainDemo(this);
	case HEAVY:
		_BotBrain = new BotBrainHeavy(this);
	case ENGI:
		_BotBrain = new BotBrainEngi(this);
	case MED:
		_BotBrain = new BotBrainMed(this);
	case SNIPER:
		_BotBrain = new BotBrainSniper(this);
	case SPY:
		_BotBrain = new BotBrainSpy(this);
	}
}

void Bot::_TFClassToJoinName(TFClass tfClass, char *tfClassName)
{
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

void Bot::_RandomClass()
{
	ChangeClass(TFClass(Util::RandomInt(0, 8)));
}