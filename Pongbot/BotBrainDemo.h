#pragma once
#include "BotBrain.h"

class BotBrainDemo : public BotBrain
{
public:
	BotBrainDemo(Bot* bot) : BotBrain(bot)
	{}

private:
	virtual void _OnThink();
	virtual void _OnSpawn();
};