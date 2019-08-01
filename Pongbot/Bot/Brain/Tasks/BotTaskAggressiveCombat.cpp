#include <stdafx.h>
#include "BotTaskAggressiveCombat.h"
#include "../TF2/Entity/Player.h"
#include "../ConVarHolder.h"
#include "../../Visibles/BotVisibles.h"
#include <hlsdk/public/edict.h>

extern const Entity _TargetEntity;
extern const WeaponSlot _MWeaponSlot;

bool BotTaskAggressiveCombat::_OnThink()
{
	Bot* bot = _GetBot();

	if (!_TargetEntity.Exists() || !bot->GetBotVisibles()->IsEntityVisible(_TargetEntity))
	{
		return true; // Removed or Gone otherwise (e.g. death)
	}

	Vector targetPos = _TargetEntity.GetPos();
	if (_TargetEntity.IsPlayer())
	{
		targetPos = Player(_TargetEntity).GetHeadPos();
	}

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
	{
		return true; // Too far away or not visible
	}
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