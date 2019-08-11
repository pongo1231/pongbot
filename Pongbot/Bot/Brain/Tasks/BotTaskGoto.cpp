#include <stdafx.h>
#include "BotTaskGoto.h"
#include "../../Bot.h"
#include "../../../Waypoint/WaypointNode.h"
#include "../../../Waypoint/WaypointManager.h"
#include "../../../TF2/Entity/EntityProvider.h"
#include "../../../TF2/Entity/EntityDataProvider.h"
#include "../../../TF2/TFTeam.h"
#include "../../../Waypoint/WaypointNodeFlagTypes.h"
#include "../../../Util.h"
#include "../../../Waypoint/WaypointNodeFlagsProvider.h"
#include "../../../ConVarHolder.h"
#include <metamod/ISmmAPI.h>
#include <stack>

extern IVEngineServer* Engine;

static bool _DrawDebugBeam = false;

bool BotTaskGoto::_OnThink()
{
	if (_Abort)
	{
		return true;
	}

	Bot* bot = _GetBot();
	Vector currentPos = bot->GetPos();
	float stuckPosRange = _ConVarHolder->CVarBotPosStuckPanicRange->GetFloat();

	if (Util::DistanceToNoZ(currentPos, _LastPos) < stuckPosRange)
	{
		_PosStuckTime++;

		float panicStuckTime = _ConVarHolder->CVarBotPosStuckPanicTime->GetFloat();
		if (_PosStuckTime > panicStuckTime + 50)
		{
			_PosStuckTime = 0;
			if (!_ShortestWay && !_NewTargetNodeStack())
			{
				// Can't get there
				return true;
			}
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
	}

	return false;
}

bool BotTaskGoto::_NewTargetNodeStack()
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
			_WaypointManager->GetShortestWaypointNodeRouteToTargetNode(closestNode, targetNode, &_WaypointNodeStack,
				nodeFlagBlacklist | _NodeFlagBlacklist);
		}
		else
		{
			_WaypointManager->GetRandomWaypointNodeRouteToTargetNode(closestNode, targetNode, &_WaypointNodeStack,
				nodeFlagBlacklist | _NodeFlagBlacklist);
		}

		if (_WaypointNodeStack.empty())
		{
			// Can't even get there, abort
			return false;
		}
		
		_TargetPosQueue = std::queue<Vector>();
		while (!_WaypointNodeStack.empty())
		{
			WaypointNode* node = _WaypointNodeStack.top();
			float nodeTouchRadius = node->GetRange();
			_TargetPosQueue.push(node->Pos + Vector(Util::RandomFloat(-nodeTouchRadius, nodeTouchRadius), Util::RandomFloat(-nodeTouchRadius, nodeTouchRadius), 0.f));
			_WaypointNodeStack.pop();
		}
		_TargetPosQueue.push(_TargetPos);
	}

	return true;
}

CON_COMMAND(pongbot_bot_goto_debug, "Draw a beam to the bots' target pos")
{
	_DrawDebugBeam = !_DrawDebugBeam;
	Util::Log(_DrawDebugBeam ? "Debug Pos Beam enabled!" : "Debug Pos Beam disabled!");
}