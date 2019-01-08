#pragma once
#include "BotTask.h"

class Bot;
class edict_t;

class BotTaskMeleeCombat : public BotTask
{
public:
	BotTaskMeleeCombat(Bot *bot, edict_t *targetEdict) : BotTask(bot), _TargetEdict(targetEdict)
	{}

private:
	edict_t *_TargetEdict;

	virtual bool _OnThink();
};