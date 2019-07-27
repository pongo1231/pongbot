#pragma once
#include "BotBrain.h"

class BotBrainPyro : public BotBrain
{
public:
	BotBrainPyro(Bot* bot) : BotBrain(bot), _IsRushingEnemy(false) {}

private:
	bool _IsRushingEnemy;

	virtual void _OnThink();
	virtual void _OnSpawn();
};