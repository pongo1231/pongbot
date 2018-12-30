#pragma once
#include "BotTask.h"
#include <queue>

class BotTaskGoto : public BotTask
{
public:
	BotTaskGoto(Vector targetPos, bool shortestWay = true, int nodeFlagBlacklist = 0);

private:
	std::queue<Vector> _TargetPosQueue;
	Vector _TargetPos;
	Vector _LastPos;
	uint8_t _PosStuckTime;
	bool _ShortestWay;
	int _NodeFlagBlacklist;

	virtual bool _OnThink();
	void _NewTargetNodeStack();
};