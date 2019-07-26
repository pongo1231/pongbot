#pragma once
#include "BotTask.h"
#include <queue>
#include <stdint.h> // uint8_t for Linux

class Bot;

class BotTaskGoto : public BotTask
{
public:
	BotTaskGoto(Bot* bot, Vector targetPos, bool shortestWay = true, int nodeFlagBlacklist = 0);

private:
	std::queue<Vector> _TargetPosQueue;
	const Vector _TargetPos;
	Vector _LastPos;
	uint8_t _PosStuckTime;
	const bool _ShortestWay;
	const int _NodeFlagBlacklist;
	float _DebugBeamDrawTime;

	virtual bool _OnThink();
	void _NewTargetNodeStack();
};