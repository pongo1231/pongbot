#pragma once
#include "BotTask.h"
#include "Player.h"
#include <queue>

class BotTaskMedHealTarget : public BotTask
{
public:
	BotTaskMedHealTarget(Bot *bot, Player targetPlayer) : BotTask(bot), _TargetHealEdict(targetPlayer.GetEdict())
	{}

private:
	edict_t *_TargetHealEdict;
	std::queue<Vector> _ChaseTargetQueue;

	virtual bool _OnThink();
};