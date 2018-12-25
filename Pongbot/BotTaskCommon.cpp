#include "BotTaskCommon.h"
#include "Util.h"
#include "WaypointNode.h"
#include "WaypointManager.h"
#include "BotVisibles.h"
#include "EntityProvider.h"
#include "EntityDataProvider.h"
#include "TFTeam.h"
#include "WaypointNodeFlagTypes.h"

#define POS_STUCK_RADIUS 1.f
#define POS_STUCK_STARTPANICTIME 120 // Bot starts crouch jumping
#define POS_STUCK_GIVEUPTIME 180 // Bot searches new path
#define WAYPOINTNODE_TOUCHED_RADIUS 5.f

extern WaypointManager *_WaypointManager;

Vector _LastPos;
unsigned int _PosStuckTime;
std::stack<WaypointNode*> _WaypointNodeStack;

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
	WaypointNode *closestNode = _WaypointManager->GetClosestWaypointNode(_LastPos);
	if (closestNode)
	{
		Bot *bot = _GetBot();
		_WaypointNodeStack = std::stack<WaypointNode*>();
		WaypointNode *targetNode = nullptr;
		int nodeFlagMask = 0;

		/* CTF */
		std::vector<edict_t*> itemFlags = _EntityProvider->SearchEdictsByClassname("item_teamflag");
		if (!itemFlags.empty())
		{
			/* TODO: Waypoint Node Flags (way less expensive than getting closest node every time!!!) */
			edict_t *allyFlag = nullptr;
			edict_t *enemyFlag = nullptr;
			for (edict_t *itemFlag : itemFlags)
				if (_EntityDataProvider->GetDataFromEdict<TFTeam>(itemFlag, TEAM) == bot->GetTeam())
					allyFlag = itemFlag;
				else
					enemyFlag = itemFlag;

			// TODO: Detect if this bot carries the flag
			if (allyFlag && _EntityDataProvider->GetDataFromEdict<int>(enemyFlag, FLAG_OWNER) != -1)
			{
				targetNode = _WaypointManager->GetClosestWaypointNode(Util::GetEdictOrigin(allyFlag));
				nodeFlagMask |= SPAWN_RED + SPAWN_BLUE;
			}
			else if (enemyFlag)
				targetNode = _WaypointManager->GetClosestWaypointNode(Util::GetEdictOrigin(enemyFlag));
		}

		if (!targetNode)
			targetNode = _WaypointManager->GetRandomWaypointNode();

		if (bot->GetTeam() == RED)
			nodeFlagMask |= SPAWN_BLUE;
		else
			nodeFlagMask |= SPAWN_RED;

		_WaypointManager->GetRandomWaypointNodeRouteToTargetNode(closestNode, targetNode, &_WaypointNodeStack,
			bot->GetTeam() == RED ? SPAWN_BLUE : SPAWN_RED);
	}
}