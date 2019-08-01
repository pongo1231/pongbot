#pragma once
#include "BotBrain.h"

class BotBrainSoldier : public BotBrain
{
public:
	BotBrainSoldier(Bot* bot) : BotBrain(bot) {}

private:
	virtual void _OnThink();
	virtual void _OnSpawn();
};