#include "BotTaskMeleeCombat.h"
#include "hlsdk/public/edict.h"
#include "Util.h"
#include "Bot.h"
#include "EntityDataProvider.h"

edict_t *_TargetEdict;

bool BotTaskMeleeCombat::_OnThink()
{
	if (!_TargetEdict || _EntityDataProvider->GetDataFromEdict<float>(_TargetEdict, DATA_HEALTH) == 0.f)
		return true; // Removed or Dead

	Bot *bot = _GetBot();
	Vector botPos = bot->GetPos();
	Vector targetPos = Util::GetEdictOrigin(_TargetEdict) + (bot->GetEarPos() - botPos); // Don't look at enemy's feet lol
	float distance = Util::DistanceToNoZ(botPos, targetPos); // Slightly faster than with Z

	if (distance > 1000.f) // TODO: Use value of convar instead of hardcoded one
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