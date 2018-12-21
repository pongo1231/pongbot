#include "BotTaskCommon.h"
#include "Util.h"
#include "WaypointNode.h"
#include "WaypointManager.h"
#include "BotVisibles.h"
#include "EntityProvider.h"
#include "EntityData.h"

#define POS_STUCK_RADIUS 1.f
#define POS_STUCK_STARTPANICTIME 120 // Bot starts crouch jumping
#define POS_STUCK_GIVEUPTIME 180 // Bot searches new path
#define WAYPOINTNODE_TOUCHED_RADIUS 5.f

extern WaypointManager *_WaypointManager;

Vector _LastPos;
unsigned int _PosStuckTime;
std::stack<WaypointNode*> _WaypointNodeStack;
WaypointNode *_ClosestWaypointNode;

BotTaskCommon::BotTaskCommon(Bot *bot) : BotTask(bot)
{}

void BotTaskCommon::OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt)
{
	Bot *bot = _GetBot();

	if (!movement)
		_DoMovement(pressedButtons, movement);
	if (!lookAt)
		_DoLooking(pressedButtons, lookAt);
}

void BotTaskCommon::_DoMovement(int *&pressedButtons, Vector2D *&movement)
{
	Bot *bot = _GetBot();
	Vector currentPos = bot->GetPos();

	if (Util::DistanceToNoZ(currentPos, _LastPos) < POS_STUCK_RADIUS)
	{
		_PosStuckTime++;
		if (_PosStuckTime > POS_STUCK_GIVEUPTIME)
		{
			_PosStuckTime = 0;
			_UpdateNewWaypointNodeStack();
		}
		else if (_PosStuckTime > POS_STUCK_STARTPANICTIME)
		{
			*pressedButtons |= IN_JUMP;
			*pressedButtons |= IN_DUCK;
		}
	}
	else
	{
		_PosStuckTime = 0;
		_LastPos = currentPos;
		_UpdateClosestWaypointNode();
	}

	if (_WaypointNodeStack.empty())
		_UpdateNewWaypointNodeStack();
	else
	{
		WaypointNode *node = _WaypointNodeStack.top();
		if (!node)
			_WaypointNodeStack.pop();
		else
		{
			Vector nodePos = node->Pos;
			if (currentPos.DistTo(nodePos) <= WAYPOINTNODE_TOUCHED_RADIUS)
				_WaypointNodeStack.pop();
			else
				movement = new Vector2D(Util::GetIdealMoveSpeedsToPos(bot, nodePos));
		}
	}
}

void BotTaskCommon::_DoLooking(int *&pressedButtons, QAngle *&lookAt)
{
	Bot *bot = _GetBot();
	BotVisibleTarget *enemyTarget = bot->GetBotVisibles()->GetMostImportantTarget(_LastPos);

	if (enemyTarget)
	{
		lookAt = new QAngle(Util::GetLookAtAngleForPos(bot, enemyTarget->Pos));
		*pressedButtons |= IN_ATTACK;
	}
	else if (_WaypointNodeStack.size() > 0)
	{
		WaypointNode *node = _WaypointNodeStack.top();
		if (node)
		{
			Vector nodeLookPos = node->Pos;
			// Don't look at the ground
			nodeLookPos.z += bot->GetEarPos().z - bot->GetPos().z;
			lookAt = new QAngle(Util::GetLookAtAngleForPos(bot, nodeLookPos));
		}
	}
}

void BotTaskCommon::_UpdateNewWaypointNodeStack()
{
	if (!_ClosestWaypointNode)
		_UpdateClosestWaypointNode();
	if (_ClosestWaypointNode) // If still nullptr, no waypoint nodes exist
	{
		_WaypointNodeStack = std::stack<WaypointNode*>();

		std::vector<edict_t*> itemFlags = _EntityProvider->SearchEdictsByClassname("item_teamflag");
		WaypointNode *targetNode = itemFlags.size() > 0
			? _WaypointManager->GetClosestWaypointNode(Util::GetEdictOrigin(itemFlags[0]))
			: _WaypointManager->GetRandomWaypointNode();

		_WaypointManager->GetWaypointNodeStackToTargetNode(_ClosestWaypointNode,
			targetNode, &_WaypointNodeStack);
	}
}

void BotTaskCommon::_UpdateClosestWaypointNode()
{
	_ClosestWaypointNode = _WaypointManager->GetClosestWaypointNode(_LastPos);
}