#include "BotTaskMeleeCombat.h"
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

BotTaskMeleeCombat::BotTaskMeleeCombat(Bot *bot, edict_t *targetEdict) : BotTask(bot), _TargetEdict(targetEdict)
{
	if (strcmp(targetEdict->GetClassName(), "player") == 0)
		_TargetPlayerInfo = IIPlayerInfoManager->GetPlayerInfo(targetEdict);
}

bool BotTaskMeleeCombat::_OnThink()
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

	if (_GetBot()->GetPos().DistTo(targetPos) > _ConVarHolder->CVarBotSecondaryWeaponDist->GetFloat())
		return true; // Too far away
	else
	{
		_OverrideBotViewAngle();
		_SetBotLookAt(targetPos);
		_BotMoveTo(targetPos);
		_AddBotPressedButton(IN_ATTACK);
	}

	return false;
}