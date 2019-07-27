#pragma once
#include "BotBrain.h"

class BotBrainSpy : public BotBrain
{
public:
	BotBrainSpy(Bot* bot) : BotBrain(bot) {}

private:
	virtual void _OnThink();
	virtual void _OnSpawn();
};