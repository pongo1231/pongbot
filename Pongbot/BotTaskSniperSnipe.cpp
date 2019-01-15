#include "BotTaskSniperSnipe.h"
#include "BotVisibles.h"
#include "Util.h"
#include "ConVarHolder.h"
#include "Player.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/edict.h>

extern IVEngineServer *Engine;

float _ShootTime;

bool BotTaskSniperSnipe::_OnThink()
{
	// TODO: Check for ammo!!!!

	Bot *bot = _GetBot();
	Player botInfo(bot->GetPlayer().GetEdict());

	BotVisibleTarget *visibleTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	Vector visibleTargetPos;
	if (visibleTarget)
	{
		visibleTargetPos = visibleTarget->Pos;

		// Abort if enemy too near
		if (visibleTarget && Util::DistanceToNoZ(bot->GetPos(), visibleTargetPos) < _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
		{
			if (botInfo.IsSniperZoomedIn())
				_AddBotPressedButton(IN_ATTACK2);
			else
				return true;
		}

		Entity targetEntity = visibleTarget->GetEntity();
		// Aim at head if it's a player
		if (targetEntity.Exists() && targetEntity.IsPlayer())
			visibleTargetPos = Player(targetEntity).GetHeadPos();

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
		_AddBotPressedButton(IN_ATTACK2);

	_OverrideBotViewAngle();
	if (visibleTarget)
		_SetBotLookAt(visibleTargetPos);
	_SetBotWeaponSlot(WEAPON_PRIMARY);

	return false;
}