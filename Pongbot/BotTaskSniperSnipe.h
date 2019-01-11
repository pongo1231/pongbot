#pragma once
#include "BotTask.h"

class BotTaskSniperSnipe : public BotTask
{
public:
	BotTaskSniperSnipe(Bot *bot) : BotTask(bot)
	{}

private:
	bool _IsZoomedIn;
	float _ShootTime;

	virtual bool _OnThink();
};