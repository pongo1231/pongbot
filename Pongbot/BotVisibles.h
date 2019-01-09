#pragma once
#include "IGameFramable.h"
#include "BotVisibleTarget.h"
#include <vector>
#include <cstdint>

class Bot;
struct edict_t;
class IServerEntity;

class BotVisibles
{
public:
	BotVisibles(Bot *bot) : _MBot(bot)
	{}

public:
	std::vector<BotVisibleTarget*> GetVisibleTargets() const;
	BotVisibleTarget *GetMostImportantTarget() const;

	void OnThink();

private:
	Bot *_MBot;
	std::vector<BotVisibleTarget*> _VisibleTargets;
	float _TickTime;

	void _AddEntity(edict_t *edict, Vector edictPos, uint8_t insertIndex);
	bool _IsTargetInSight(Vector targetPos) const;
	bool _HasClearLineToTarget(IServerEntity *targetEntity, Vector targetPos) const;
};