#include "BotTaskAggressiveCombat.h"
#include "Util.h"
#include "EntityDataProvider.h"
#include "ConVarHolder.h"
#include "BotVisibles.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <hlsdk/public/edict.h>

extern IPlayerInfoManager *IIPlayerInfoManager;
extern IServerGameClients *IIServerGameClients;

edict_t *_TargetEdict;
WeaponSlot _MWeaponSlot;

bool BotTaskAggressiveCombat::_OnThink()
{
	Bot *bot = _GetBot();

	if (!_TargetEdict || !bot->GetBotVisibles()->IsEntityVisible(_TargetEdict))
		return true; // Removed or Gone otherwise (e.g. death)

	Vector targetPos = Util::GetEdictOrigin(_TargetEdict);
	if (strcmp(_TargetEdict->GetClassName(), "player") == 0)
	{
		// Don't shoot at player's feet
		Vector earPos;
		IIServerGameClients->ClientEarPosition(_TargetEdict, &earPos);
		targetPos += (earPos - targetPos) / 2;
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