#pragma once
#include <hlsdk/public/mathlib/mathlib.h>

enum BotTargetPriority {
	HIGH,
	NORMAL,
	LOW,
	FRIENDLY
};

struct BotVisibleTarget {
	const Vector Pos;
	const BotTargetPriority Priority;

	BotVisibleTarget(Vector pos, BotTargetPriority priority) : Pos(pos), Priority(priority) {}
};