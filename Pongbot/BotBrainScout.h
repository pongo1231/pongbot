#pragma once
#include "BotBrain.h"

class BotBrainScout : public BotBrain
{
public:
	BotBrainScout(Bot *bot) : BotBrain(bot)
	{}

private:
	virtual void _OnThink();
	virtual void _OnSpawn();
};