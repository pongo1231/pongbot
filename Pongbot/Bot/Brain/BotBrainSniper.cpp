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
	if (visibleTarget.IsValid()
		&& Util::DistanceToNoZ(bot->GetPos(), visibleTarget.GetPos()) > _ConVarHolder->CVarBotWeaponLongRangeDist->GetFloat()
		&& !_IsCurrentBotTaskOfType(typeid(BotTaskSniperSnipe)))
	{
		_SetBotTask(new BotTaskSniperSnipe(bot));
	}
}