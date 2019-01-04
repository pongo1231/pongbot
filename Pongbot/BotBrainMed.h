#pragma once
#include "BotBrain.h"

class BotBrainMed : public BotBrain
{
public:
	BotBrainMed(Bot *bot) : BotBrain(bot)
	{}

private:
	virtual void _OnThink();
};