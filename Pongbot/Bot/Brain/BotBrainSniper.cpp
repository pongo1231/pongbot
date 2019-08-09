#include <stdafx.h>
#include "BotBrainSniper.h"
#include "../Visibles/BotVisibles.h"
#include "../../ConVarHolder.h"
#include "Tasks/BotTaskSniperSnipe.h"
#include "../../Util.h"

void BotBrainSniper::_OnThink()
{
	Bot* bot = _GetBot();

	BotVisibleTarget visibleTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	if (visibleTarget.IsValid())
	{
		if (Util::DistanceToNoZ(bot->GetPos(), visibleTarget.GetPos()) > _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
		{
			if (!_IsBotSniping)
			{
				_IsBotSniping = true;
				_SetBotTask(new BotTaskSniperSnipe(bot));
			}
		}
		else
		{
			_IsBotSniping = false;
		}
	}
}

void BotBrainSniper::_OnSpawn()
{
	_IsBotSniping = false;
}