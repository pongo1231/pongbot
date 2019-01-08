#pragma once
#include <hlsdk/public/mathlib/mathlib.h>

class edict_t;

enum BotTargetPriority
{
	PRIORITY_FRIENDLY = -1,
	PRIORITY_LOW,
	PRIORITY_NORMAL,
	PRIORITY_HIGH
};

struct BotVisibleTarget
{
	const Vector Pos;
	const BotTargetPriority Priority;
	edict_t *Edict;

	BotVisibleTarget(Vector pos, BotTargetPriority priority, edict_t *edict) : Pos(pos), Priority(priority), Edict(edict)
	{}
};