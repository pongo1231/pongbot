#include "BotTaskGoto.h"
#include "Bot.h"
#include "WaypointNode.h"
#include "WaypointManager.h"
#include "EntityProvider.h"
#include "EntityDataProvider.h"
#include "TFTeam.h"
#include "WaypointNodeFlagTypes.h"
#include "Util.h"
#include "WaypointNodeFlagsProvider.h"
#include <metamod/ISmmAPI.h>
#include <stack>

#define POS_STUCK_RADIUS 5.f
#define POS_STUCK_STARTPANICTIME 120 // Bot starts crouch jumping
#define WAYPOINTNODE_TOUCHED_RADIUS 5.f
#define WAYPOINTNODE_FIND_CLOSEST_MAXDIST 500.f
#define TARGETPOS_DEBUG_BEAM_TICK 0.1f

extern IVEngineServer *Engine;

static bool _DrawDebugBeam = false;

std::queue<Vector> _TargetPosQueue;
Vector _TargetPos;
Vector _LastPos;
uint8_t _PosStuckTime;
bool _ShortestWay;
int _NodeFlagBlacklist;
float _DebugBeamDrawTime;

BotTaskGoto::BotTaskGoto(Bot *bot, Vector targetPos, bool shortestWay, int nodeFlagBlacklist) : BotTask(bot),
	_TargetPos(targetPos), _ShortestWay(shortestWay), _NodeFlagBlacklist(nodeFlagBlacklist)
{
	_NewTargetNodeStack();
}

bool BotTaskGoto::_OnThink()
{
	Bot *bot = _GetBot();
	Vector currentPos = bot->GetPos();

	if (Util::DistanceToNoZ(currentPos, _LastPos) < POS_STUCK_RADIUS)
	{
		_PosStuckTime++;
		if (_PosStuckTime > POS_STUCK_STARTPANICTIME + 50)
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
			_SetBotLookAt(targetPos + (bot->GetEarPos() - currentPos));
		}

		if (_DrawDebugBeam && Engine->Time() > _DebugBeamDrawTime)
		{
			Util::DrawBeam(bot->GetEarPos(), targetPos, 255, 0, 0, TARGETPOS_DEBUG_BEAM_TICK);
			_DebugBeamDrawTime = Engine->Time() + TARGETPOS_DEBUG_BEAM_TICK;
		}
	}

	return false;
}

void BotTaskGoto::_NewTargetNodeStack()
{
	Bot *bot = _GetBot();
	Vector currentPos = bot->GetPos();
	WaypointNode *closestNode = _WaypointManager->GetClosestWaypointNode(currentPos);
	if (closestNode)
	{
		WaypointNode *targetNode = _WaypointManager->GetClosestWaypointNode(_TargetPos);
		int nodeFlagBlacklist = _WaypointNodeFlagsProvider->GetInaccessibleNodeFlagsForBot(bot);

		if (!targetNode)
			targetNode = _WaypointManager->GetRandomWaypointNode();

		std::stack<WaypointNode*> _WaypointNodeStack;
		if (_ShortestWay)
			_WaypointManager->GetShortestWaypointNodeRouteToTargetNode(closestNode, targetNode, &_WaypointNodeStack,
				nodeFlagBlacklist | _NodeFlagBlacklist);
		else
			_WaypointManager->GetRandomWaypointNodeRouteToTargetNode(closestNode, targetNode, &_WaypointNodeStack,
				nodeFlagBlacklist | _NodeFlagBlacklist);
		
		_TargetPosQueue = std::queue<Vector>();
		while (!_WaypointNodeStack.empty())
		{
			_TargetPosQueue.push(_WaypointNodeStack.top()->Pos);
			_WaypointNodeStack.pop();
		}
		_TargetPosQueue.push(_TargetPos);
	}
}

CON_COMMAND(pongbot_bot_goto_debug, "Draw a beam to the bots' target pos")
{
	_DrawDebugBeam = !_DrawDebugBeam;
	Util::Log(_DrawDebugBeam ? "Debug Pos Beam enabled!" : "Debug Pos Beam disabled!");
}