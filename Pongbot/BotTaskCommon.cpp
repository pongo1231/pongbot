#include "BotTaskCommon.h"
#include "Util.h"
#include "WaypointNode.h"
#include "WaypointManager.h"
#include "BotVisibles.h"

#define POS_STUCK_RADIUS 0.2
#define POS_STUCK_STARTPANICTIME 120
#define POS_STUCK_GIVEUPTIME 180
#define WAYPOINTNODE_TOUCHED_RADIUS 5

extern WaypointManager *_WaypointManager;

Vector _LastPos;
unsigned int _PosStuckTime;
std::stack<WaypointNode*> _WaypointNodeStack;
WaypointNode *_ClosestWaypointNode;

BotTaskCommon::BotTaskCommon(Bot *bot) : BotTask(bot) {}

void BotTaskCommon::OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt, int *&taskFlags) {
	Bot *bot = _GetBot();
	Vector currentPos = bot->GetPos();

	if (Util::DistanceToNoZ(currentPos, _LastPos) < POS_STUCK_RADIUS) {
		_PosStuckTime++;

		if (_PosStuckTime > POS_STUCK_GIVEUPTIME) {
			_PosStuckTime = 0;
			_UpdateNewWaypointNodeStack();
		}
		else if (_PosStuckTime > POS_STUCK_STARTPANICTIME) {
			*pressedButtons |= IN_JUMP;
			*pressedButtons |= IN_DUCK;
		}
	}
	else {
		_PosStuckTime = 0;
		_LastPos = currentPos;
		_UpdateClosestWaypointNode();
	}

	if (_WaypointNodeStack.empty())
		_UpdateNewWaypointNodeStack();
	else {
		WaypointNode *node = _WaypointNodeStack.top();

		if (!node) // Pop if node doesn't exist anymore
			_WaypointNodeStack.pop();
		else {
			Vector nodePos = node->Pos;

			if (currentPos.DistTo(nodePos) <= WAYPOINTNODE_TOUCHED_RADIUS)
				_WaypointNodeStack.pop();
			else
				movement = new Vector2D(Util::GetIdealMoveSpeedsToPos(bot, nodePos));

			//lookAt = new QAngle(Util::GetLookAtAngleForPos(_Bot, Vector(nodePos.x, nodePos.y, _Bot->GetEarPos().z)));
			// TEMP!!!
			if (!lookAt) {
				std::vector<edict_t*> visibleEdicts = bot->GetBotVisibles()->GetVisibleEdicts();

				if (visibleEdicts.size() > 0) {
					lookAt = new QAngle(Util::GetLookAtAngleForPos(bot, Util::GetEdictOrigin(
						visibleEdicts[Util::RandomInt(0, visibleEdicts.size() - 1)])));

					*pressedButtons |= IN_ATTACK;
				}
			}
		}
	}
}

void BotTaskCommon::_UpdateNewWaypointNodeStack() {
	if (!_ClosestWaypointNode)
		_UpdateClosestWaypointNode();
	if (_ClosestWaypointNode) { // If still nullptr, no waypoint nodes exist
		_WaypointNodeStack = std::stack<WaypointNode*>();
		_WaypointManager->GetWaypointNodeStackToTargetNode(_ClosestWaypointNode,
			_WaypointManager->GetRandomWaypointNode(), &_WaypointNodeStack);
	}
}

void BotTaskCommon::_UpdateClosestWaypointNode() {
	_ClosestWaypointNode = _WaypointManager->GetClosestWaypointNode(_LastPos);
}