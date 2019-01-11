#include "BotTaskSniperSnipe.h"
#include "BotVisibles.h"
#include "Util.h"
#include "ConVarHolder.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/edict.h>

extern IServerGameClients *IIServerGameClients;
extern IVEngineServer *Engine;

bool _IsZoomedIn;
float _ShootTime;

bool BotTaskSniperSnipe::_OnThink()
{
	// TODO: Check for ammo!!!!

	Bot *bot = _GetBot();

	BotVisibleTarget *visibleTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	Vector visibleTargetPos;
	if (visibleTarget)
	{
		visibleTargetPos = visibleTarget->Pos;

		// Abort if enemy too near
		if (visibleTarget && Util::DistanceToNoZ(bot->GetPos(), visibleTargetPos) < _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
		{
			if (_IsZoomedIn)
				_AddBotPressedButton(IN_ATTACK2);
			return true;
		}

		edict_t *targetEdict = visibleTarget->Edict;
		// Aim at head if it's a player
		if (targetEdict && strcmp(targetEdict->GetClassName(), "player") == 0)
			IIServerGameClients->ClientEarPosition(targetEdict, &visibleTargetPos);

		// Only shoot after a little time
		float engineTime = Engine->Time();
		if (engineTime > _ShootTime)
		{
			_ShootTime = engineTime + Util::RandomFloat(1.5f, 5.f);
			_AddBotPressedButton(IN_ATTACK);
		}
	}

	// Zoom in if not already done so
	if (!_IsZoomedIn)
	{
		_IsZoomedIn = true;
		_AddBotPressedButton(IN_ATTACK2);
	}

	_OverrideBotViewAngle();
	if (visibleTarget)
		_SetBotLookAt(visibleTargetPos);
	_SetBotWeaponSlot(WEAPON_PRIMARY);
}