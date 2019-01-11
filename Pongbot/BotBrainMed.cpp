#include "BotBrainMed.h"
#include "BotVisibles.h"
#include "BotTaskMedHealTarget.h"
#include "Util.h"
#include "ConVarHolder.h"
#include <hlsdk/public/edict.h>

edict_t *_CurrentHealTarget;

void BotBrainMed::_OnThink()
{
	Bot *bot = _GetBot();
	std::queue<BotTask*> newTaskQueue;

	BotVisibles *botVisibles = bot->GetBotVisibles();
	if (_CurrentHealTarget && !botVisibles->IsEntityVisible(_CurrentHealTarget))
		_CurrentHealTarget = nullptr;

	if (!_CurrentHealTarget)
	{
		Vector botPos = bot->GetPos();
		std::vector<BotVisibleTarget*> botVisibleTargets = botVisibles->GetVisibleTargets();
		// TODO: Target most important target for healing instead of first one
		for (BotVisibleTarget *visibleTarget : botVisibleTargets)
			if (visibleTarget->Edict && strcmp(visibleTarget->Edict->GetClassName(), "player") == 0
				&& visibleTarget->Priority == PRIORITY_FRIENDLY
				&& botPos.DistTo(visibleTarget->Pos) < _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
			{
				_CurrentHealTarget = visibleTarget->Edict;
				newTaskQueue.push(new BotTaskMedHealTarget(bot, _CurrentHealTarget));
				break;
			}
	}

	if (!newTaskQueue.empty())
		SetTaskQueue(newTaskQueue);
}

void BotBrainMed::_OnSpawn()
{
	_CurrentHealTarget = nullptr;
}