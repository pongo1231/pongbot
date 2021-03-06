#include <stdafx.h>
#include "BotBrainMed.h"
#include "../Visibles/BotVisibles.h"
#include "Tasks/BotTaskMedHealTarget.h"
#include "../../Util.h"
#include "../../ConVarHolder.h"
#include "../../TF2/Entity/Player.h"
#include <hlsdk/public/edict.h>

void BotBrainMed::_OnThink()
{
	Bot* bot = _GetBot();

	BotVisibles* botVisibles = bot->GetBotVisibles();
	if (!_IsCurrentBotTaskOfType(typeid(BotTaskMedHealTarget))
		|| (_CurrentHealTarget && !botVisibles->IsEntityVisible(_CurrentHealTarget)))
	{
		_CurrentHealTarget = nullptr;
	}

	if (!_CurrentHealTarget)
	{
		Vector botPos = bot->GetPos();
		std::vector<BotVisibleTarget> botVisibleTargets = botVisibles->GetVisibleTargets();
		// TODO: Target most important target for healing instead of first one
		for (BotVisibleTarget visibleTarget : botVisibleTargets)
		{
			Entity visibleEntity = visibleTarget.GetEntity();
			if (visibleEntity.Exists() && visibleEntity.IsPlayer() && visibleTarget.GetPriority() == PRIORITY_FRIENDLY
				&& botPos.DistTo(visibleTarget.GetPos()) < _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
			{
				_CurrentHealTarget = visibleEntity.GetEdict();
				_SetBotTask(new BotTaskMedHealTarget(bot, _CurrentHealTarget));
				break;
			}
		}
	}
}

void BotBrainMed::_OnSpawn()
{
	_CurrentHealTarget = nullptr;
}