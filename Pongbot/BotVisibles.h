#pragma once
#include "IGameFramable.h"
#include "BotVisibleTarget.h"
#include <hlsdk/public/edict.h>
#include <vector>
#include <cstdint>

class Bot;

class BotVisibles
{
public:
	BotVisibles(Bot *bot);

public:
	std::vector<BotVisibleTarget*> GetVisibleTargets() const;
	BotVisibleTarget *GetMostImportantTarget() const;

	void OnThink();

private:
	Bot *_MBot;
	std::vector<BotVisibleTarget*> _VisibleTargets;
	float _TickTime;

	void _AddEntity(edict_t *edict, Vector edictPos, uint8_t insertIndex);
};