#pragma once
#include "BotTask.h"

class BotTaskSniperSnipe : public BotTask
{
public:
	BotTaskSniperSnipe(Bot *bot) : BotTask(bot), _ShootTime(0.f)
	{}

private:
	float _ShootTime;

	virtual bool _OnThink();
};