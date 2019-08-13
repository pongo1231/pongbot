#pragma once
#include "BotTask.h"
#include <queue>

class Bot;

class BotTaskGoto : public BotTask
{
public:
	BotTaskGoto(Bot* bot, Vector targetPos, bool shortestWay = true, int nodeFlagBlacklist = 0) : BotTask(bot),
		_TargetPos(targetPos), _ShortestWay(shortestWay), _NodeFlagBlacklist(nodeFlagBlacklist),
		_DebugBeamDrawTime(0.f)
	{
		_AbortTask = !_NewTargetNodeStack();
	}

private:
	std::queue<Vector> _TargetPosQueue;
	const Vector _TargetPos;
	const bool _ShortestWay;
	const int _NodeFlagBlacklist;
	float _DebugBeamDrawTime;

	bool _NewTargetNodeStack();

	virtual bool _OnThink();
	virtual void _OnBotStuckPanic();
	virtual void _OnBotDefinitelyStuck();
};