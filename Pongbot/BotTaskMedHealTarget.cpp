#include "BotTaskMedHealTarget.h"
#include "BotVisibles.h"
#include "Util.h"
#include "ConVarHolder.h"

edict_t *_TargetHealEdict;
std::queue<Vector> _ChaseTargetQueue;

bool BotTaskMedHealTarget::_OnThink()
{
	Bot *bot = _GetBot();

	if (!_TargetHealEdict)
		return true; // Target disappeared

	Vector botPos = bot->GetPos();
	Vector targetEdictPos = Util::GetEdictOrigin(_TargetHealEdict);

	if (botPos.DistTo(targetEdictPos) > _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
		return true; // Target is too far away

	float botDistanceToTarget = _ConVarHolder->CVarBotMedTargetDistance->GetFloat();

	// Only walk near target pos
	Vector targetPos = _ChaseTargetQueue.empty() ? botPos : _ChaseTargetQueue.front();
	if (Util::DistanceToNoZ(botPos, targetPos) > botDistanceToTarget) // Ignore z coordinate (e.g. jumping)
		_BotMoveTo(targetPos);
	else if (!_ChaseTargetQueue.empty())
		_ChaseTargetQueue.pop();

	// Walk target's path even if target isn't visible before giving up
	if (bot->GetBotVisibles()->IsEntityVisible(_TargetHealEdict))
	{
		Vector lastTargetPos = _ChaseTargetQueue.empty() ? botPos : _ChaseTargetQueue.back();
		if (lastTargetPos.DistTo(targetEdictPos) > botDistanceToTarget && _ChaseTargetQueue.size() < 10)
			_ChaseTargetQueue.push(targetEdictPos);
	}
	else if (_ChaseTargetQueue.empty())
		return true;

	_OverrideBotViewAngle();
	_SetBotLookAt(targetEdictPos); // TODO: Don't look at feet as if the bot has a feet fetish
	_SetBotWeaponSlot(WEAPON_SECONDARY);
	_AddBotPressedButton(IN_ATTACK);
	_AddBotPressedButton(IN_ATTACK2); // TODO: Only uber if there's a reason to

	return false;
}