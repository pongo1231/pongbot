#pragma once
#include "IGameFramable.h"
#include "Entity.h"
#include <hlsdk/public/mathlib/vector.h>
#include <vector>
#include <cstdint>

class Bot;
struct edict_t;
class IServerEntity;

enum BotTargetPriority
{
	PRIORITY_FRIENDLY = -1,
	PRIORITY_LOW,
	PRIORITY_NORMAL,
	PRIORITY_HIGH
};

struct BotVisibleTarget
{
	BotVisibleTarget(Vector pos, BotTargetPriority priority, Entity entity) : Pos(pos), Priority(priority), Entity(entity)
	{}

	const Vector Pos;
	const BotTargetPriority Priority;
	Entity Entity;
};

class BotVisibles
{
public:
	BotVisibles(Bot *bot) : _MBot(bot)
	{}

public:
	std::vector<BotVisibleTarget*> GetVisibleTargets() const;
	BotVisibleTarget *GetMostImportantTarget() const;
	bool IsEntityVisible(Entity entity) const;

	void OnThink();

private:
	Bot *_MBot;
	std::vector<BotVisibleTarget*> _VisibleTargets;
	float _TickTime;

	void _AddEntity(Entity entity, Vector edictPos, uint8_t insertIndex);
	bool _IsTargetInSight(Vector targetPos) const;
	bool _HasClearLineToTarget(IServerEntity *targetEntity, Vector targetPos) const;
};