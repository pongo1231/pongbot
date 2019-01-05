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

ConVar _CVarNodeTouchRadius("pongbot_bot_goto_nodetouchradius", "50.0", 0, "Min distance to node to count as 'touched'");
ConVar _CVarPosStuckPanicTime("pongbot_bot_goto_stuckpanictime", "120.0", 0, "Time until bot starts panicing if stuck (crouch jump)");
ConVar _CVarTargetPosDebugBeamTick("pongbot_bot_goto_debugbeamtick", "0.1", 0, "How often the goto debug beam gets updated");

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
	float nodeTouchRadius = _CVarNodeTouchRadius.GetFloat();

	if (Util::DistanceToNoZ(currentPos, _LastPos) < nodeTouchRadius)
	{
		_PosStuckTime++;

		float panicStuckTime = _CVarPosStuckPanicTime.GetFloat();
		if (_PosStuckTime > panicStuckTime + 50)
		{
			_PosStuckTime = 0;
			if (!_ShortestWay)
				_NewTargetNodeStack();
		}
		else if (_PosStuckTime > panicStuckTime)
		{
			_AddBotPressedButton(IN_JUMP);
			_AddBotPressedButton(IN_DUCK);
			bot->ExecClientCommand("voicemenu 2 5");
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
		if (currentPos.DistTo(targetPos) <= nodeTouchRadius)
			_TargetPosQueue.pop();
		else
		{
			_BotMoveTo(targetPos);
			_SetBotLookAt(targetPos + (bot->GetEarPos() - currentPos));
		}

		if (_DrawDebugBeam && Engine->Time() > _DebugBeamDrawTime)
		{
			float debugBeamTick = _CVarTargetPosDebugBeamTick.GetFloat();
			Util::DrawBeam(bot->GetEarPos(), targetPos, 255, 0, 0, debugBeamTick);
			_DebugBeamDrawTime = Engine->Time() + debugBeamTick;
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
			float nodeTouchRadius = _CVarNodeTouchRadius.GetFloat();
			_TargetPosQueue.push(_WaypointNodeStack.top()->Pos
				+ Vector(Util::RandomFloat(-nodeTouchRadius, nodeTouchRadius),
					Util::RandomFloat(-nodeTouchRadius, nodeTouchRadius), 0.f));
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