#pragma once
#include "BotBrain.h"

class BotBrainEngi : public BotBrain
{
public:
	BotBrainEngi(Bot* bot) : BotBrain(bot) {}

private:
	virtual void _OnThink();
	virtual void _OnSpawn();
};