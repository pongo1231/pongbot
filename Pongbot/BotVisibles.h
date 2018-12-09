#pragma once
#include "GameFramable.h"
#include <hlsdk/public/edict.h>
#include <vector>

class Bot;

class BotVisibles {
public:
	BotVisibles(Bot *bot);

public:
	std::vector<edict_t*> GetVisibleEdicts() const;

	void OnThink();

private:
	Bot *_MBot;
	std::vector<edict_t*> _VisibleEdicts;
	float _TickTime;
};