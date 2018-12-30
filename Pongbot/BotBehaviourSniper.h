#pragma once
#include "BotBehaviour.h"

class BotBehaviourSniper : public BotBehaviour
{
public:
	BotBehaviourSniper(Bot *bot) : BotBehaviour(bot)
	{}

private:
	virtual void _OnThink();
};