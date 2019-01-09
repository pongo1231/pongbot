#include "BotTaskAggressiveCombat.h"
#include "Util.h"
#include "EntityDataProvider.h"
#include "ConVarHolder.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <hlsdk/public/edict.h>

extern IPlayerInfoManager *IIPlayerInfoManager;
extern IServerGameClients *IIServerGameClients;

edict_t *_TargetEdict;
IPlayerInfo *_TargetPlayerInfo;
WeaponSlot _MWeaponSlot;

BotTaskAggressiveCombat::BotTaskAggressiveCombat(Bot *bot, edict_t *targetEdict,
	WeaponSlot weaponSlot) : BotTask(bot), _TargetEdict(targetEdict), _MWeaponSlot(weaponSlot)
{
	if (strcmp(targetEdict->GetClassName(), "player") == 0)
		_TargetPlayerInfo = IIPlayerInfoManager->GetPlayerInfo(targetEdict);
}

bool BotTaskAggressiveCombat::_OnThink()
{
	if (!_TargetEdict || _EntityDataProvider->GetDataFromEdict<float>(_TargetEdict, DATA_HEALTH) == 0.f
		|| (_TargetPlayerInfo && _TargetPlayerInfo->IsDead()))
		return true; // Removed or Dead

	Vector targetPos = Util::GetEdictOrigin(_TargetEdict);
	if (_TargetPlayerInfo)
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

	if (_GetBot()->GetPos().DistTo(targetPos) > maxDistance)
		return true; // Too far away
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