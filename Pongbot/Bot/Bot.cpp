#include <stdafx.h>
#include "Bot.h"
#include "BotManager.h"
#include "../Util.h"
#include "Brain/BotBrain.h"
#include "Visibles/BotVisibles.h"
#include "Brain/BotBrainScout.h"
#include "Brain/BotBrainSoldier.h"
#include "Brain/BotBrainPyro.h"
#include "Brain/BotBrainDemo.h"
#include "Brain/BotBrainHeavy.h"
#include "Brain/BotBrainEngi.h"
#include "Brain/BotBrainMed.h"
#include "Brain/BotBrainSniper.h"
#include "Brain/BotBrainSpy.h"
#include "../ConVarHolder.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <string>

extern IVEngineServer* Engine;
extern IBotManager* IIBotManager;
extern IServerPluginHelpers* IIServerPluginHelpers;
extern IPlayerInfoManager* IIPlayerInfoManager;

Bot::Bot(Player player, const char* name) : Name(name), _Player(player), _Edict(player.GetEdict()),
	_IBotController(IIBotManager->GetBotController(_Edict)),
	_ClassInfo(_TFClassInfoProvider->GetClassInfo(TFClass::CLASS_UNK)),
	_IPlayerInfo(IIPlayerInfoManager->GetPlayerInfo(_Edict)), _BotVisibles(new BotVisibles(this)),
	_BotBrain(nullptr), _Movement(Vector2D()), _TargetViewAngle(QAngle()), _PressedButtons(0),
	_SelectedWeaponSlot(WEAPON_UNKNOWN)
{
	_SwitchToFittingTeam();
	_RandomClass();

	ExecClientCommand("cl_autorezoom 0");

	Util::DebugLog("Bot fully initialized!");
}

Bot::~Bot()
{
	delete _BotVisibles;
	delete _BotBrain;
}

void Bot::Think()
{
	if (!Exists())
	{
		return;
	}

	if (_BotVisibles && _ConVarHolder->CVarBotEnableVisibility->GetBool())
	{
		_BotVisibles->OnThink();
	}
	if (_BotBrain && _ConVarHolder->CVarBotEnableBrain->GetBool())
	{
		_BotBrain->OnThink();
	}
	
	// Smoothed out aiming
	QAngle currentViewAngle = GetViewAngle();
	QAngle finalViewAngle = Util::CorrectViewAngle(_TargetViewAngle - currentViewAngle)
		/ _ConVarHolder->CVarBotAimSensivity->GetFloat() + currentViewAngle;
	finalViewAngle.x *= 2; // Pitch should be doubled

	CBotCmd cmd;
	cmd.buttons = _PressedButtons;
	if (_Movement.IsValid())
	{
		cmd.forwardmove = _Movement.x;
		cmd.sidemove = _Movement.y;
	}
	if (finalViewAngle.IsValid())
	{
		cmd.viewangles = finalViewAngle;
	}
	_IBotController->RunPlayerMove(&cmd);
}

const Player Bot::GetPlayer() const
{
	return _Player;
}

edict_t *Bot::GetEdict() const
{
	return _Edict;
}

bool Bot::Exists() const
{
	return GetPlayer().Exists();
}

Vector Bot::GetPos() const
{
	return GetPlayer().GetPos();
}

Vector Bot::GetEarPos() const
{
	return _Player.GetHeadPos();
}

QAngle Bot::GetViewAngle() const
{
	return _IBotController->GetLocalAngles();
}

void Bot::SetViewAngle(QAngle angle)
{
	if (angle.IsValid())
	{
		_TargetViewAngle = angle;
	}
}

TFClass Bot::GetClass() const
{
	return _CurrentClass;
}

TFTeam Bot::GetTeam() const
{
	return GetPlayer().GetTeam();
}

BotVisibles* Bot::GetBotVisibles() const
{
	return _BotVisibles;
}

void Bot::SetMovement(Vector2D movement)
{
	if (movement.IsValid())
	{
		_Movement = movement;
	}
}

Vector2D Bot::GetMovement() const
{
	return _Movement;
}

void Bot::SetPressedButtons(int pressedButtons)
{
	_PressedButtons = pressedButtons;
}

int Bot::GetPressedButtons() const
{
	return _PressedButtons;
}

WeaponSlot Bot::GetSelectedWeaponSlot() const
{
	return _SelectedWeaponSlot;
}

void Bot::SetSelectedWeapon(WeaponSlot weapon)
{
	if (!_ClassInfo.IsValid())
	{
		return;
	}

	const char *weaponName = nullptr;
	switch (weapon)
	{
	case WEAPON_PRIMARY:
		weaponName = _ClassInfo.GetPrimary().GetWeaponName();
		break;
	case WEAPON_SECONDARY:
		weaponName = _ClassInfo.GetSecondary().GetWeaponName();
		break;
	case WEAPON_MELEE:
		weaponName = _ClassInfo.GetMelee().GetWeaponName();
		break;
	}

	if (weaponName)
	{
		_SelectedWeaponSlot = weapon;
		ExecClientCommand("use %s", weaponName);
	}
}

bool Bot::IsDead() const
{
	return GetPlayer().IsDead();
}

void Bot::ChangeClass(TFClass tfClass)
{
	ExecClientCommand("joinclass %s", _TFClassToJoinName(tfClass));

	_CurrentClass = tfClass;
	_ClassInfo = _TFClassInfoProvider->GetClassInfo(tfClass);
	_UpdateBotBrain();
}

void Bot::ExecClientCommand(const char* command, ...) const
{
	char fullCommand[128];
	va_list args;
	va_start(args, command);
	vsnprintf(fullCommand, sizeof(fullCommand), command, args);
	va_end(args);

	IIServerPluginHelpers->ClientCommand(GetEdict(), fullCommand);
}

WeaponSlot Bot::GetIdealWeaponForRange(float range) const
{
	// Determine ideal weapons for ranges first
	// TODO: Take ammo into account
	TFClassInfoWeapon weaponInfos[] =
	{
		_ClassInfo.GetPrimary(),
		_ClassInfo.GetSecondary(),
		_ClassInfo.GetMelee()
	};
	WeaponSlot longRangeWeaponSlot = WEAPON_PRIMARY;
	WeaponSlot middleRangeWeaponSlot = WEAPON_SECONDARY;
	WeaponSlot shortRangeWeaponSlot = WEAPON_MELEE;
	for (int i = 0; i < 3; i++)
	{
		unsigned int weaponFlags = weaponInfos[i].GetWeaponFlags();
		if (weaponFlags & WEAPONFLAG_PRIORITIZE_LONGDIST)
		{
			longRangeWeaponSlot = (WeaponSlot)i;
		}
		if (weaponFlags & WEAPONFLAG_PRIORITIZE_MIDDLEDIST)
		{
			middleRangeWeaponSlot = (WeaponSlot)i;
		}
		if (weaponFlags & WEAPONFLAG_PRIORITIZE_SHORTDIST)
		{
			shortRangeWeaponSlot = (WeaponSlot)i;
		}
	}

	if (range < _ConVarHolder->CVarBotWeaponMiddleRangeDist->GetFloat())
	{
		return shortRangeWeaponSlot;
	}
	else if (range < _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
	{
		return middleRangeWeaponSlot;
	}
	else
	{
		return longRangeWeaponSlot;
	}
}

void Bot::_SwitchToFittingTeam()
{
	uint8_t red = 0;
	uint8_t blue = 0;
	for (Player player : Util::GetAllPlayers())
	{
		TFTeam team = player.GetTeam();
		if (team == TEAM_RED)
		{
			red++;
		}
		else if (team == TEAM_BLUE)
		{
			blue++;
		}
	}

	_IPlayerInfo->ChangeTeam(blue < red ? TEAM_BLUE : TEAM_RED);
}

void Bot::_UpdateBotBrain()
{
	delete _BotBrain;
	switch (_CurrentClass)
	{
	case SCOUT:
		_BotBrain = new BotBrainScout(this);
		break;
	case SOLDIER:
		_BotBrain = new BotBrainSoldier(this);
		break;
	case PYRO:
		_BotBrain = new BotBrainPyro(this);
		break;
	case DEMO:
		_BotBrain = new BotBrainDemo(this);
		break;
	case HEAVY:
		_BotBrain = new BotBrainHeavy(this);
		break;
	case ENGI:
		_BotBrain = new BotBrainEngi(this);
		break;
	case MED:
		_BotBrain = new BotBrainMed(this);
		break;
	case SNIPER:
		_BotBrain = new BotBrainSniper(this);
		break;
	case SPY:
		_BotBrain = new BotBrainSpy(this);
		break;
	}

	Util::DebugLog("Created new bot brain (%s) for bot %s (Edict Index: %d)", _TFClassToJoinName(GetClass()), Name, GetEdict()->m_iIndex);
}

const char* Bot::_TFClassToJoinName(TFClass tfClass) const
{
	switch (tfClass) {
	case SCOUT:
		return "scout";
	case SOLDIER:
		return "soldier";
	case PYRO:
		return "pyro";
	case DEMO:
		return "demoman";
	case HEAVY:
		return "heavyweapons";
	case ENGI:
		return "engineer";
	case MED:
		return "medic";
	case SNIPER:
		return "sniper";
	case SPY:
		return "spy";
	}

	return "random";
}

void Bot::_RandomClass()
{
	ChangeClass(TFClass(Util::RandomInt(0, 8)));
}