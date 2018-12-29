#include "Bot.h"
#include "BotManager.h"
#include "Util.h"
#include "BotTaskMaster.h"
#include "BotVisibles.h"
#include "BotTaskMasterCollection.h"
#include "TFTeam.h"
#include "TFClass.h"
#include "EntityDataProvider.h"
#include "Util.h"
#include <metamod/ISmmPlugin.h>
#include <string>

// Higher = slower
#define BOT_AIM_SENSITIVITY_X 3.f
#define BOT_AIM_SENSITIVITY_Y 1.5f

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
QAngle _LookAt;
bool _IsDead;

Bot::Bot(edict_t *edict, const char *name) : Name(name), _Edict(edict),
	_IIBotController(IIBotManager->GetBotController(edict)),
	_IIPlayerInfo(IIPlayerInfoManager->GetPlayerInfo(edict))
{
	_BotVisibles = new BotVisibles(this);
	_IsDead = false;

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
	delete _BotTaskMaster;
	delete _BotVisibles;
}

void Bot::Think()
{
	// Update Task Master on respawn
	if (IsDead())
	{
		_IsDead = true;
		return;
	}
	else if (_IsDead)
	{
		_UpdateTaskMaster();
		_IsDead = false;
	}

	_BotVisibles->OnThink();

	int pressedButtons = 0;
	Vector2D *movement = nullptr;
	QAngle *targetLookAt = nullptr;
	if (_BotTaskMaster)
		_BotTaskMaster->OnThink(&pressedButtons, movement, targetLookAt);
	_HandleAiming(targetLookAt);

	CBotCmd cmd;
	cmd.buttons = pressedButtons;
	if (movement)
	{
		cmd.forwardmove = movement->x;
		cmd.sidemove = movement->y;
	}
	cmd.viewangles = _LookAt;
	_IIBotController->RunPlayerMove(&cmd);

	delete movement;
	delete targetLookAt;
}

void Bot::_HandleAiming(QAngle *targetLookAt)
{
	if (targetLookAt)
	{
		static QAngle previousLookAt;
		if (*targetLookAt != previousLookAt)
		{
			previousLookAt = *targetLookAt;
			_LookAt = *targetLookAt;
		}
	}

	QAngle currentLookAt = GetAngle();
	QAngle targetAngleDistance = Util::CorrectViewAngle(_LookAt - currentLookAt);
	_LookAt = QAngle(currentLookAt.x + targetAngleDistance.x / BOT_AIM_SENSITIVITY_X,
		currentLookAt.y + targetAngleDistance.y / BOT_AIM_SENSITIVITY_Y, 0.f);
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
	return _IIPlayerInfo->GetAbsOrigin();
}

Vector Bot::GetEarPos() const
{
	Vector earPos;
	IIServerGameClients->ClientEarPosition(_Edict, &earPos);
	return earPos;
}

QAngle Bot::GetAngle() const
{
	return _IIPlayerInfo->GetAbsAngles();
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
	_UpdateTaskMaster();
}

void Bot::ExecClientCommand(const char *command) const
{
	IIServerPluginHelpers->ClientCommand(_Edict, command);
}

void Bot::_UpdateTaskMaster()
{
	delete _BotTaskMaster;
	switch (_CurrentClass)
	{
	case SCOUT:
		_BotTaskMaster = new BotTaskMasterScout(this);
	case SOLDIER:
		_BotTaskMaster = new BotTaskMasterSoldier(this);
	case PYRO:
		_BotTaskMaster = new BotTaskMasterPyro(this);
	case DEMO:
		_BotTaskMaster = new BotTaskMasterDemo(this);
	case HEAVY:
		_BotTaskMaster = new BotTaskMasterHeavy(this);
	case ENGI:
		_BotTaskMaster = new BotTaskMasterEngi(this);
	case MED:
		_BotTaskMaster = new BotTaskMasterMed(this);
	case SNIPER:
		_BotTaskMaster = new BotTaskMasterSniper(this);
	case SPY:
		_BotTaskMaster = new BotTaskMasterSpy(this);
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