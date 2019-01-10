#include "BotTaskMedHealTarget.h"
#include "BotVisibles.h"
#include "Util.h"
#include "ConVarHolder.h"

edict_t *_TargetHealEdict;
std::queue<Vector> _ChaseTargetQueue;

bool BotTaskMedHealTarget::_OnThink()
{
	Bot *bot = _GetBot();

	if (!_TargetHealEdict || !bot->GetBotVisibles()->IsEntityVisible(_TargetHealEdict))
		return true; // Target disappeared

	Vector botPos = bot->GetPos();
	Vector targetEdictPos = Util::GetEdictOrigin(_TargetHealEdict);

	if (botPos.DistTo(targetEdictPos) > _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
		return true; // Target is too far away

	float botDistanceToTarget = _ConVarHolder->CVarBotMedTargetDistance->GetFloat();

	Vector targetPos = _ChaseTargetQueue.empty() ? Vector() : _ChaseTargetQueue.front();
	if (targetPos.DistTo(targetEdictPos) > botDistanceToTarget && _ChaseTargetQueue.size() < 3)
		_ChaseTargetQueue.push(targetEdictPos);

	// Only walk near target pos
	if (botPos.DistTo(targetPos) > botDistanceToTarget)
		_BotMoveTo(targetPos);
	else if (!_ChaseTargetQueue.empty())
		_ChaseTargetQueue.pop();

	_OverrideBotViewAngle();
	_SetBotLookAt(targetEdictPos); // TODO: Don't look at feet as if the bot has a feet fetish
	_SetBotWeaponSlot(WEAPON_SECONDARY);
	_AddBotPressedButton(IN_ATTACK);
	_AddBotPressedButton(IN_ATTACK2); // TODO: Only uber if there's a reason to

	return false;
}