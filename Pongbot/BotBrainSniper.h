#pragma once
#include "BotBrain.h"

class BotBrainSniper : public BotBrain
{
public:
	BotBrainSniper(Bot *bot) : BotBrain(bot)
	{}

private:
	virtual void _OnThink();
};