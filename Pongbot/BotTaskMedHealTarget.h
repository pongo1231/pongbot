#pragma once
#include "BotTask.h"
#include <queue>

class BotTaskMedHealTarget : public BotTask
{
public:
	BotTaskMedHealTarget(Bot *bot, edict_t *targetEdict) : BotTask(bot), _TargetHealEdict(targetEdict)
	{}

private:
	edict_t *_TargetHealEdict;
	std::queue<Vector> _ChaseTargetQueue;

	virtual bool _OnThink();
};