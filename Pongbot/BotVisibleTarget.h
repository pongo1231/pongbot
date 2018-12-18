#pragma once
#include <hlsdk/public/mathlib/mathlib.h>

enum BotTargetPriority
{
	FRIENDLY = -1,
	LOW,
	NORMAL,
	HIGH
};

struct BotVisibleTarget
{
	const Vector Pos;
	const BotTargetPriority Priority;

	BotVisibleTarget(Vector pos, BotTargetPriority priority) : Pos(pos), Priority(priority)
	{}
};