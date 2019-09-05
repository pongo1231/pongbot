#include <stdafx.h>
#include "BotTaskSniperSnipe.h"
#include "../../Visibles/BotVisibles.h"
#include "../../../Util.h"
#include "../../../ConVarHolder.h"
#include "../../../TF2/Entity/Player.h"
#include "../../../Waypoint/WaypointManager.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/edict.h>

extern WaypointManager* _WaypointManager;

bool BotTaskSniperSnipe::_OnThink()
{
	// TODO: Check for ammo!!!!

	Bot* bot = _GetBot();
	Vector botPos = bot->GetPos();
	Player botInfo(bot->GetEdict());

	BotVisibleTarget visibleTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	Vector visibleTargetPos;

	// Zoom in if not already done so
	if (!botInfo.IsSniperZoomedIn())
	{
		_AddBotPressedButton(IN_ATTACK2);
	}

	if (!visibleTarget.IsValid())
	{
		if (Engine->Time() > _CampTime)
		{
			return true;
		}
		else if (_PrefAngle != QAngle(0.f, 0.f, 0.f))
		{
			_SetBotAngle(_PrefAngle);
		}
	}
	else
	{
		visibleTargetPos = visibleTarget.GetPos();

		// Abort if enemy too near
		if (Util::DistanceToNoZ(botPos, visibleTargetPos) < _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
		{
			return true;
		}

		Entity targetEntity = visibleTarget.GetEntity();
		// Aim at head if it's a player
		if (targetEntity.Exists() && targetEntity.IsPlayer())
		{
			visibleTargetPos = Player(targetEntity).GetHeadPos();
		}

		// Only shoot after a little time
		float engineTime = Engine->Time();
		if (engineTime > _ShootTime)
		{
			_ShootTime = engineTime + Util::RandomFloat(20.f, 40.f);
			_AddBotPressedButton(IN_ATTACK);
		}
	}

	_OverrideBotViewAngle();
	if (visibleTarget.IsValid())
	{
		_SetBotLookAt(visibleTargetPos);
	}
	_SetBotWeaponSlot(WEAPON_PRIMARY);

	return false;
}

void BotTaskSniperSnipe::_OnStop()
{
	if (Player(_GetBot()->GetEdict()).IsSniperZoomedIn())
	{
		// Don't walk around zoomed in lol
		_AddBotPressedButton(IN_ATTACK2);
	}
}