#include "BotTaskGoto.h"
#include "WaypointNode.h"
#include "WaypointManager.h"
#include "EntityProvider.h"
#include "EntityDataProvider.h"
#include "TFTeam.h"
#include "WaypointNodeFlagTypes.h"
#include "Util.h"
#include <stack>

#define POS_STUCK_RADIUS 5.f
#define POS_STUCK_STARTPANICTIME 120 // Bot starts crouch jumping
#define WAYPOINTNODE_TOUCHED_RADIUS 5.f
#define WAYPOINTNODE_FIND_CLOSEST_MAXDIST 500.f

std::queue<Vector> _TargetPosQueue;
Vector _TargetPos;
Vector _LastPos;
uint8_t _PosStuckTime;
bool _ShortestWay;
int _NodeFlagBlacklist;

BotTaskGoto::BotTaskGoto(Vector targetPos, bool shortestWay, int nodeFlagBlacklist) : _TargetPos(targetPos),
	_ShortestWay(shortestWay), _NodeFlagBlacklist(nodeFlagBlacklist)
{
	_NewTargetNodeStack();
}

bool BotTaskGoto::_OnThink()
{
	Vector currentPos = _GetBotPos();

	if (Util::DistanceToNoZ(currentPos, _LastPos) < POS_STUCK_RADIUS)
	{
		_PosStuckTime++;
		if (_PosStuckTime > POS_STUCK_STARTPANICTIME + 50.f)
		{
			_PosStuckTime = 0;
			if (!_ShortestWay)
				_NewTargetNodeStack();
		}
		else if (_PosStuckTime > POS_STUCK_STARTPANICTIME)
		{
			_AddBotPressedButton(IN_JUMP);
			_AddBotPressedButton(IN_DUCK);
		}
	}
	else
	{
		_PosStuckTime = 0;
		_LastPos = currentPos;
	}

	if (_TargetPosQueue.empty())
		return true;
	else
	{
		Vector targetPos = _TargetPosQueue.front();
		if (currentPos.DistTo(targetPos) <= WAYPOINTNODE_TOUCHED_RADIUS)
			_TargetPosQueue.pop();
		else
		{
			_BotMoveTo(targetPos);
			_SetBotLookAt(targetPos + (_GetBotEarPos() - currentPos));
		}
	}

	return false;
}

void BotTaskGoto::_NewTargetNodeStack()
{
	Vector currentPos = _GetBotPos();
	WaypointNode *closestNode = _WaypointManager->GetClosestWaypointNode(currentPos);
	if (closestNode)
	{
		WaypointNode *targetNode = _WaypointManager->GetClosestWaypointNode(_TargetPos);
		int nodeFlagBlacklist = 0;

		if (!targetNode)
			targetNode = _WaypointManager->GetRandomWaypointNode();

		if (_GetBotTeam() == RED)
			nodeFlagBlacklist |= SPAWN_BLUE;
		else
			nodeFlagBlacklist |= SPAWN_RED;

		std::stack<WaypointNode*> _WaypointNodeStack;
		if (_ShortestWay)
			_WaypointManager->GetShortestWaypointNodeRouteToTargetNode(closestNode, targetNode, &_WaypointNodeStack,
				nodeFlagBlacklist + _NodeFlagBlacklist);
		else
			_WaypointManager->GetRandomWaypointNodeRouteToTargetNode(closestNode, targetNode, &_WaypointNodeStack,
				nodeFlagBlacklist + _NodeFlagBlacklist);
		
		_TargetPosQueue = std::queue<Vector>();
		while (!_WaypointNodeStack.empty())
		{
			_TargetPosQueue.push(_WaypointNodeStack.top()->Pos);
			_WaypointNodeStack.pop();
		}
		_TargetPosQueue.push(_TargetPos);
	}
}