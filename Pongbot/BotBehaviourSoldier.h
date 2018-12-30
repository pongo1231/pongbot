#pragma once
#include "BotBehaviour.h"

class BotBehaviourSoldier : public BotBehaviour
{
public:
	BotBehaviourSoldier(Bot *bot) : BotBehaviour(bot)
	{}

private:
	virtual void _OnThink();
};