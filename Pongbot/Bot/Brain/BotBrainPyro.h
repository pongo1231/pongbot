#pragma once
#include "BotBrain.h"

class BotBrainPyro : public BotBrain
{
public:
	BotBrainPyro(Bot* bot) : BotBrain(bot) {}

private:
	virtual void _OnThink();
};