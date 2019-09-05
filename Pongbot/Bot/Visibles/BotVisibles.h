#pragma once
#include "../../IGameFramable.h"
#include "../../TF2/Entity/Entity.h"
#include <hlsdk/public/mathlib/vector.h>
#include <vector>
#include <cstdint>

class Bot;
struct edict_t;
class IServerEntity;

enum BotTargetPriority
{
	PRIORITY_UNK = -2,
	PRIORITY_FRIENDLY,
	PRIORITY_LOW,
	PRIORITY_NORMAL,
	PRIORITY_HIGH
};

struct BotVisibleTarget
{
	BotVisibleTarget() : _Priority(PRIORITY_UNK), _Valid(false) {}
	BotVisibleTarget(Vector pos, BotTargetPriority priority, Entity entity) : _Pos(pos), _Priority(priority), _Entity(entity), _Valid(true) {}
	BotVisibleTarget(const BotVisibleTarget& target)
	{
		_Pos = target.GetPos();
		_Priority = target.GetPriority();
		_Entity = target.GetEntity();
		_Valid = target.IsValid();
	}
	BotVisibleTarget& operator=(BotVisibleTarget&& target)
	{
		_Pos = target.GetPos();
		_Priority = target.GetPriority();
		_Entity = target.GetEntity();
		_Valid = target.IsValid();
	}

	inline Vector GetPos() const
	{
		return _Pos;
	}

	inline BotTargetPriority GetPriority() const
	{
		return _Priority;
	}

	inline Entity GetEntity() const
	{
		return _Entity;
	}

	inline bool IsValid() const
	{
		return _Valid;
	}

private:
	Vector _Pos;
	BotTargetPriority _Priority;
	Entity _Entity;
	bool _Valid;
};

class BotVisibles
{
public:
	BotVisibles(Bot* bot) : _MBot(bot), _TickTime(0.f) {}

public:
	std::vector<BotVisibleTarget> GetVisibleTargets() const;
	BotVisibleTarget GetMostImportantTarget() const;
	bool IsEntityVisible(Entity entity) const;

	void OnThink();

private:
	const Bot* _MBot;
	std::vector<BotVisibleTarget> _VisibleTargets;
	float _TickTime;

	void _AddEntity(Entity entity, Vector edictPos, uint8_t insertIndex);
	bool _IsTargetInSight(Vector targetPos) const;
	bool _HasClearLineToTarget(IServerEntity *targetEntity, Vector targetPos) const;
};