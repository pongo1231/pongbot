#pragma once
#include "BotBrain.h"

class Player;

class BotBrainMed : public BotBrain
{
public:
	BotBrainMed(Bot* bot) : BotBrain(bot)
	{}

private:
	edict_t* _CurrentHealTarget;

	virtual void _OnThink();
	virtual void _OnSpawn();
};