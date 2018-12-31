#include "Bot.h"
#include "BotManager.h"
#include "Util.h"
#include "BotBehaviour.h"
#include "BotVisibles.h"
#include "BotBehaviourScout.h"
#include "BotBehaviourSoldier.h"
#include "BotBehaviourPyro.h"
#include "BotBehaviourDemo.h"
#include "BotBehaviourHeavy.h"
#include "BotBehaviourEngi.h"
#include "BotBehaviourMed.h"
#include "BotBehaviourSniper.h"
#include "BotBehaviourSpy.h"
#include "TFTeam.h"
#include "TFClass.h"
#include "EntityDataProvider.h"
#include "Util.h"
#include <metamod/ISmmPlugin.h>
#include <string>

#define BOT_AIM_SENSITIVITY 3.f

extern IVEngineServer *Engine;
extern IBotManager *IIBotManager;
extern IPlayerInfoManager *IIPlayerInfoManager;
extern IServerPluginHelpers *IIServerPluginHelpers;
extern IServerGameClients *IIServerGameClients;

const char *Name;
edict_t *_Edict;
IBotController *_IIBotController;
IPlayerInfo *_IIPlayerInfo;
BotBehaviour *_BotBehaviour;
BotVisibles *_BotVisibles;
TFClass _CurrentClass;
QAngle _TargetViewAngle;
Vector2D _Movement;
int _PressedButtons;

Bot::Bot(edict_t *edict, const char *name) : Name(name), _Edict(edict),
	_IIBotController(IIBotManager->GetBotController(edict)),
	_IIPlayerInfo(IIPlayerInfoManager->GetPlayerInfo(edict))
{
	_BotBehaviour = new BotBehaviour(this);
	_BotVisibles = new BotVisibles(this);

	// Join team with least players or red team if both equal
	uint8_t red = 0;
	uint8_t blue = 0;
	for (edict_t *edict : Util::GetAllPlayers())
	{
		TFTeam team = _EntityDataProvider->GetDataFromEdict<TFTeam>(edict, TEAM);
		if (team == RED)
			red++;
		else if (team == BLUE)
			blue++;
	}
	_IIPlayerInfo->ChangeTeam(blue < red ? BLUE : RED);

	_RandomClass();
}

Bot::~Bot()
{
	delete _BotVisibles;
	delete _BotBehaviour;
}

void Bot::Think()
{
	_BotVisibles->OnThink();
	_BotBehaviour->OnThink();
	
	// Smoothed out aiming
	QAngle currentViewAngle = GetViewAngle();
	QAngle finalViewAngle = Util::CorrectViewAngle(_TargetViewAngle - currentViewAngle) / BOT_AIM_SENSITIVITY + currentViewAngle;
	finalViewAngle.x *= 2;

	CBotCmd cmd;
	cmd.buttons = _PressedButtons;
	cmd.forwardmove = _Movement.x;
	cmd.sidemove = _Movement.y;
	cmd.viewangles = finalViewAngle;
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

bool Bot::IsDead() const
{
	return _IIPlayerInfo->IsDead();
}

void Bot::ChangeClass(TFClass tfClass)
{
	char newClass[16];
	_TFClassToJoinName(tfClass, newClass);

	char cmd[32];
	sprintf_s(cmd, "joinclass %s", newClass);
	IIServerPluginHelpers->ClientCommand(_Edict, cmd);

	_CurrentClass = tfClass;
	_UpdateBotBehaviour();
}

void Bot::ExecClientCommand(const char *command) const
{
	IIServerPluginHelpers->ClientCommand(_Edict, command);
}

void Bot::_UpdateBotBehaviour()
{
	delete _BotBehaviour;
	switch (_CurrentClass)
	{
	case SCOUT:
		_BotBehaviour = new BotBehaviourScout(this);
	case SOLDIER:
		_BotBehaviour = new BotBehaviourSoldier(this);
	case PYRO:
		_BotBehaviour = new BotBehaviourPyro(this);
	case DEMO:
		_BotBehaviour = new BotBehaviourDemo(this);
	case HEAVY:
		_BotBehaviour = new BotBehaviourHeavy(this);
	case ENGI:
		_BotBehaviour = new BotBehaviourEngi(this);
	case MED:
		_BotBehaviour = new BotBehaviourMed(this);
	case SNIPER:
		_BotBehaviour = new BotBehaviourSniper(this);
	case SPY:
		_BotBehaviour = new BotBehaviourSpy(this);
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