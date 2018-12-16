#pragma once
#include "GameFramable.h"
#include "BotVisibleTarget.h"
#include <hlsdk/public/edict.h>
#include <vector>

class Bot;

class BotVisibles {
public:
	BotVisibles(Bot *bot);

public:
	std::vector<BotVisibleTarget*> GetVisibleTargets() const;

	void OnThink();

private:
	Bot *_MBot;
	std::vector<BotVisibleTarget*> _VisibleTargets;
};