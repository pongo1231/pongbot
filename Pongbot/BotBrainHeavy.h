#pragma once
#include "BotBrain.h"

class BotBrainHeavy : public BotBrain
{
public:
	BotBrainHeavy(Bot *bot) : BotBrain(bot)
	{}

private:
	virtual void _OnThink();
	virtual void _OnSpawn();
};