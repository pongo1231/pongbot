#include "BotBrainSniper.h"
#include "BotVisibles.h"
#include "ConVarHolder.h"
#include "Util.h"
#include "BotTaskSniperSnipe.h"

bool _IsBotSniping;

void BotBrainSniper::_OnThink()
{
	Bot *bot = _GetBot();
	std::queue<BotTask*> newTaskQueue;

	BotVisibleTarget *visibleTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	if (visibleTarget)
	{
		if (Util::DistanceToNoZ(bot->GetPos(), visibleTarget->Pos) > _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
		{
			if (!_IsBotSniping)
			{
				_IsBotSniping = true;
				newTaskQueue.push(new BotTaskSniperSnipe(bot));
			}
		}
		else
			_IsBotSniping = false;
	}

	if (!newTaskQueue.empty())
		SetTaskQueue(newTaskQueue);
}

void BotBrainSniper::_OnSpawn()
{
	_IsBotSniping = false;
}