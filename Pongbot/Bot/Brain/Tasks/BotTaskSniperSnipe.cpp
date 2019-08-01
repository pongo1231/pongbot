#include <stdafx.h>
#include "BotTaskSniperSnipe.h"
#include "../../Visibles/BotVisibles.h"
#include "../Util.h"
#include "../ConVarHolder.h"
#include "../TF2/Entity/Player.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/edict.h>

extern IVEngineServer* Engine;

bool BotTaskSniperSnipe::_OnThink()
{
	// TODO: Check for ammo!!!!

	Bot* bot = _GetBot();
	Player botInfo(bot->GetEdict());

	BotVisibleTarget* visibleTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	Vector visibleTargetPos;

	if (!visibleTarget)
	{
		return true;
	}
	else
	{
		visibleTargetPos = visibleTarget->Pos;

		// Abort if enemy too near
		if (/*visibleTarget &&*/ Util::DistanceToNoZ(bot->GetPos(), visibleTargetPos) < _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
		{
			return true;
		}

		Entity targetEntity = visibleTarget->GetEntity();
		// Aim at head if it's a player
		if (targetEntity.Exists() && targetEntity.IsPlayer())
		{
			visibleTargetPos = Player(targetEntity).GetHeadPos();
		}

		// Only shoot after a little time
		float engineTime = Engine->Time();
		if (engineTime > _ShootTime)
		{
			_ShootTime = engineTime + Util::RandomFloat(3.f, 10.f);
			_AddBotPressedButton(IN_ATTACK);
		}
	}

	// Zoom in if not already done so
	if (!botInfo.IsSniperZoomedIn())
	{
		_AddBotPressedButton(IN_ATTACK2);
	}

	_OverrideBotViewAngle();
	if (visibleTarget)
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