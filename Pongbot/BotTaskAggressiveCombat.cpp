#include "BotTaskAggressiveCombat.h"
#include "Util.h"
#include "Player.h"
#include "ConVarHolder.h"
#include "BotVisibles.h"
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <hlsdk/public/edict.h>

edict_t *_TargetEdict;
WeaponSlot _MWeaponSlot;

bool BotTaskAggressiveCombat::_OnThink()
{
	Bot *bot = _GetBot();
	Entity targetEntity = Entity(_TargetEdict);

	if (!_TargetEdict || !bot->GetBotVisibles()->IsEntityVisible(targetEntity))
		return true; // Removed or Gone otherwise (e.g. death)

	Vector targetPos = Entity(_TargetEdict).GetPos();
	if (targetEntity.IsPlayer())
		targetPos += Player(targetEntity).GetHeadPos();

	// Determine max distance to target before task aborts
	float maxDistance;
	switch (_MWeaponSlot)
	{
	case WEAPON_PRIMARY:
		maxDistance = _ConVarHolder->CVarBotMaxVisibleDist->GetFloat();
		break;
	case WEAPON_SECONDARY:
		maxDistance = _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat();
		break;
	case WEAPON_MELEE:
		maxDistance = _ConVarHolder->CVarBotWeaponMiddleRangeDist->GetFloat();
		break;
	}

	if (bot->GetPos().DistTo(targetPos) > maxDistance)
		return true; // Too far away or not visible
	else
	{
		_OverrideBotViewAngle();
		_SetBotLookAt(targetPos);
		_BotMoveTo(targetPos);
		_SetBotWeaponSlot(_MWeaponSlot);
		_AddBotPressedButton(IN_ATTACK);
	}

	return false;
}