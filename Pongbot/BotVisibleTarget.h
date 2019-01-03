#pragma once
#include <hlsdk/public/mathlib/mathlib.h>

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

	BotVisibleTarget(Vector pos, BotTargetPriority priority) : Pos(pos), Priority(priority)
	{}
};