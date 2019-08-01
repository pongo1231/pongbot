#include <stdafx.h>
#include "BotTaskMedHealTarget.h"
#include "../../Visibles/BotVisibles.h"
#include "../Util.h"
#include "../ConVarHolder.h"

bool BotTaskMedHealTarget::_OnThink()
{
	Bot* bot = _GetBot();

	if (!_TargetHealPlayer.Exists())
	{
		return true; // Target disappeared
	}

	Vector botPos = bot->GetPos();
	Vector targetPlayerPos = _TargetHealPlayer.GetPos();

	if (botPos.DistTo(targetPlayerPos) > _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
	{
		return true; // Target is too far away
	}

	float botDistanceToTarget = _ConVarHolder->CVarBotMedTargetDistance->GetFloat();

	// Only walk near target pos
	Vector targetPos = _ChaseTargetQueue.empty() ? botPos : _ChaseTargetQueue.front();
	if (Util::DistanceToNoZ(botPos, targetPos) > botDistanceToTarget) // Ignore z coordinate (e.g. jumping)
	{
		_BotMoveTo(targetPos);
	}
	else if (!_ChaseTargetQueue.empty())
	{
		_ChaseTargetQueue.pop();
	}

	// Walk target's path even if target isn't visible before giving up
	if (bot->GetBotVisibles()->IsEntityVisible(_TargetHealPlayer))
	{
		Vector lastTargetPos = _ChaseTargetQueue.empty() ? botPos : _ChaseTargetQueue.back();
		if (lastTargetPos.DistTo(targetPlayerPos) > botDistanceToTarget && _ChaseTargetQueue.size() < 10)
		{
			_ChaseTargetQueue.push(targetPlayerPos);
		}
	}
	else if (_ChaseTargetQueue.empty())
	{
		return true;
	}

	_OverrideBotViewAngle();
	_SetBotLookAt(_TargetHealPlayer.GetHeadPos());
	_SetBotWeaponSlot(WEAPON_SECONDARY);
	_AddBotPressedButton(IN_ATTACK);
	_AddBotPressedButton(IN_ATTACK2); // TODO: Only uber if there's a reason to

	return false;
}