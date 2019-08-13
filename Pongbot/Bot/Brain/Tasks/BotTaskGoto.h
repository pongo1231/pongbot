#pragma once
#include "BotTask.h"
#include <queue>

class Bot;

class BotTaskGoto : public BotTask
{
public:
	BotTaskGoto(Bot* bot, Vector targetPos, bool shortestWay = true, bool switchToShortestWayOnTargetPosVisible = true,
		int nodeFlagBlacklist = 0) : BotTask(bot), _TargetPos(targetPos), _ShortestWay(shortestWay),
		_SwitchToShortestWayOnTargetPosVisible(switchToShortestWayOnTargetPosVisible),
		_NodeFlagBlacklist(nodeFlagBlacklist), _DebugBeamDrawTime(0.f)
	{
		_NewTargetNodeStack();
	}

private:
	std::queue<Vector> _TargetPosQueue;
	const Vector _TargetPos;
	bool _ShortestWay;
	const bool _SwitchToShortestWayOnTargetPosVisible;
	const int _NodeFlagBlacklist;
	float _DebugBeamDrawTime;

	void _NewTargetNodeStack();

	virtual bool _OnThink();
	virtual void _OnBotStuckPanic();
	virtual void _OnBotDefinitelyStuck();
};