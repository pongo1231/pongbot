#include "BotBrainSniper.h"
#include "BotVisibles.h"
#include "ConVarHolder.h"
#include "BotTaskSniperSnipe.h"
#include "Util.h"

bool _IsBotSniping;

void BotBrainSniper::_OnThink()
{
	Bot *bot = _GetBot();

	BotVisibleTarget *visibleTarget = bot->GetBotVisibles()->GetMostImportantTarget();
	if (visibleTarget)
	{
		if (Util::DistanceToNoZ(bot->GetPos(), visibleTarget->Pos) > _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat())
		{
			if (!_IsBotSniping)
			{
				_IsBotSniping = true;
				_SetBotTask(new BotTaskSniperSnipe(bot));
			}
		}
		else
			_IsBotSniping = false;
	}
}

void BotBrainSniper::_OnSpawn()
{
	_IsBotSniping = false;
}