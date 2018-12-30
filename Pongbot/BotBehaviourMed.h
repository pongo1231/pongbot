#pragma once
#include "BotBehaviour.h"

class BotBehaviourMed : public BotBehaviour
{
public:
	BotBehaviourMed(Bot *bot) : BotBehaviour(bot)
	{}

private:
	virtual void _OnThink();
};