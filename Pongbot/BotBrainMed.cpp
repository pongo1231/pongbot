#include "BotBrainMed.h"
#include "BotVisibles.h"
#include "BotTaskMedHealTarget.h"
#include "Util.h"
#include "ConVarHolder.h"
#include "Player.h"
#include <hlsdk/public/edict.h>

edict_t *_CurrentHealTarget;

void BotBrainMed::_OnThink()
{
	Bot *bot = _GetBot();

	BotVisibles *botVisibles = bot->GetBotVisibles();
	if (_CurrentHealTarget && !botVisibles->IsEntityVisible(_CurrentHealTarget))
		_CurrentHealTarget = nullptr;

	if (!_CurrentHealTarget)
	{
		Vector botPos = bot->GetPos();
		std::vector<BotVisibleTarget*> botVisibleTargets = botVisibles->GetVisibleTargets();
		// TODO: Target most important target for healing instead of first one
		for (BotVisibleTarget *visibleTarget : botVisibleTargets)
		{
			Entity visibleEntity = visibleTarget->Entity;
			if (visibleEntity.Exists() && visibleEntity.IsPlayer() && visibleTarget->Priority == PRIORITY_FRIENDLY
				&& botPos.DistTo(visibleTarget->Pos) < _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
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