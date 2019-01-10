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
	if (_HasState(BOTSTATE_MED_HEALING) && !botVisibles->IsEntityVisible(_CurrentHealTarget))
		_RemoveState(BOTSTATE_MED_HEALING);

	if (!_HasState(BOTSTATE_MED_HEALING))
	{
		Vector botPos = bot->GetPos();
		std::vector<BotVisibleTarget*> botVisibleTargets = botVisibles->GetVisibleTargets();
		// TODO: Target most important target for healing instead of first one
		for (BotVisibleTarget *visibleTarget : botVisibleTargets)
			if (visibleTarget->Edict && strcmp(visibleTarget->Edict->GetClassName(), "player") == 0
				&& visibleTarget->Priority == PRIORITY_FRIENDLY
				&& botPos.DistTo(visibleTarget->Pos) < _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
			{
				_AddState(BOTSTATE_MED_HEALING);
				_CurrentHealTarget = visibleTarget->Edict;
				newTaskQueue.push(new BotTaskMedHealTarget(bot, _CurrentHealTarget));
				break;
			}
	}

	if (!newTaskQueue.empty())
		SetTaskQueue(newTaskQueue);
}