#pragma once
#include "BotTask.h"
#include "../TF2/Entity/Player.h"
#include <queue>

class BotTaskMedHealTarget : public BotTask
{
public:
	BotTaskMedHealTarget(Bot* bot, Player targetPlayer) : BotTask(bot), _TargetHealPlayer(targetPlayer) {}

private:
	const Player _TargetHealPlayer;
	std::queue<Vector> _ChaseTargetQueue;

	virtual bool _OnThink();
};