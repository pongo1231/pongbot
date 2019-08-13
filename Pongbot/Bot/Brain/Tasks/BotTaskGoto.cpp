#include <stdafx.h>
#include "BotTaskGoto.h"
#include "../../Bot.h"
#include "../../../Waypoint/WaypointNode.h"
#include "../../../Waypoint/WaypointManager.h"
#include "../../../TF2/Entity/EntityProvider.h"
#include "../../../TF2/Entity/EntityDataProvider.h"
#include "../../../TF2/TFTeam.h"
#include "../../../TF2/Trace/TraceFilters.h"
#include "../../../Waypoint/WaypointNodeFlagTypes.h"
#include "../../../Util.h"
#include "../../../Waypoint/WaypointNodeFlagsProvider.h"
#include "../../../ConVarHolder.h"
#include <metamod/ISmmAPI.h>
#include <stack>

extern IVEngineServer* Engine;

static bool _DrawDebugBeam = false;

void BotTaskGoto::_NewTargetNodeStack()
{
	Bot* bot = _GetBot();
	Vector currentPos = bot->GetPos();
	WaypointNode* closestNode = _WaypointManager->GetClosestWaypointNode(currentPos);
	if (closestNode)
	{
		WaypointNode* targetNode = _WaypointManager->GetClosestWaypointNode(_TargetPos);
		int nodeFlagBlacklist = _WaypointNodeFlagsProvider->GetInaccessibleNodeFlagsForBot(bot);

		if (!targetNode)
		{
			targetNode = _WaypointManager->GetRandomWaypointNode();
		}

		std::stack<WaypointNode*> _WaypointNodeStack;
		if (_ShortestWay)
		{
			_WaypointManager->GetShortestWaypointNodeRouteToTargetNode(closestNode, targetNode,
				&_WaypointNodeStack, nodeFlagBlacklist | _NodeFlagBlacklist);
		}
		else
		{
			_WaypointManager->GetRandomWaypointNodeRouteToTargetNode(closestNode, targetNode,
				&_WaypointNodeStack, nodeFlagBlacklist | _NodeFlagBlacklist);
		}

		if (_WaypointNodeStack.empty())
		{
			// Can't even get there, abort
			_AbortTask = true;
			return;
		}
		
		_TargetPosQueue = std::queue<Vector>();
		while (!_WaypointNodeStack.empty())
		{
			WaypointNode* node = _WaypointNodeStack.top();
			float nodeTouchRadius = node->GetRange();
			_TargetPosQueue.push(node->Pos + Vector(Util::RandomFloat(-nodeTouchRadius, nodeTouchRadius),
				Util::RandomFloat(-nodeTouchRadius, nodeTouchRadius), 0.f));
			_WaypointNodeStack.pop();
		}
		_TargetPosQueue.push(_TargetPos);
	}
}

bool BotTaskGoto::_OnThink()
{
	Bot* bot = _GetBot();
	Vector currentPos = bot->GetPos();

	if (_TargetPosQueue.empty())
	{
		return true;
	}
	else
	{
		Vector targetPos = _TargetPosQueue.front();
		if (Util::DistanceToNoZ(currentPos, targetPos) <= 5.f)
		{
			_TargetPosQueue.pop();
		}
		else
		{
			_BotMoveTo(targetPos);
			_SetBotLookAt(targetPos + (bot->GetEarPos() - currentPos));
		}

		if (_DrawDebugBeam && Engine->Time() > _DebugBeamDrawTime)
		{
			float debugBeamTick = _ConVarHolder->CVarBotTargetPosDebugBeamTick->GetFloat();
			Util::DrawBeam(bot->GetEarPos(), targetPos, 255, 0, 0, debugBeamTick);
			_DebugBeamDrawTime = Engine->Time() + debugBeamTick;
		}

		// Switch to shortest way mode if target pos is visible and switchToShortestWayOnTargetPosVisible is on
		if (_SwitchToShortestWayOnTargetPosVisible && !_ShortestWay)
		{
			trace_t traceResult;
			Vector tracePos = targetPos;
			tracePos.z += bot->GetEarPos().z;
			Util::TraceLine(currentPos, tracePos, MASK_SOLID | MASK_OPAQUE,
				&TraceFilterSimple(bot->GetIServerEntity(), nullptr), &traceResult);
			if (!traceResult.DidHit())
			{
				_ShortestWay = true;
				_NewTargetNodeStack();
			}
		}
	}

	return false;
}

void BotTaskGoto::_OnBotStuckPanic()
{
	// Try changing waypoint node position offset maybe
	Vector& nextTargetPos = _TargetPosQueue.front();
	WaypointNode* node = _WaypointManager->GetClosestWaypointNode(nextTargetPos);
	float nodeTouchRadius = node->GetRange();
	nextTargetPos = Vector(node->Pos + Vector(Util::RandomFloat(-nodeTouchRadius, nodeTouchRadius),
		Util::RandomFloat(-nodeTouchRadius, nodeTouchRadius), 0.f));
}

void BotTaskGoto::_OnBotDefinitelyStuck()
{
	_NewTargetNodeStack();
}

CON_COMMAND(pongbot_bot_goto_debug, "Draw a beam to the bots' target pos")
{
	_DrawDebugBeam = !_DrawDebugBeam;
	Util::Log(_DrawDebugBeam ? "Debug Pos Beam enabled!" : "Debug Pos Beam disabled!");
}