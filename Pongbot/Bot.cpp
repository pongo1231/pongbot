#include "Bot.h"
#include "BotManager.h"
#include "Util.h"
#include "BotHelper.h"
#include "WaypointManager.h"
#include <metamod/ISmmPlugin.h>
#include <hlsdk/game/shared/in_buttons.h>
#include <string>

#define POS_STUCK_RADIUS 0.2
#define POS_STUCK_STARTPANICTIME 60
#define POS_STUCK_GIVEUPTIME 120
#define WAYPOINTNODE_TOUCHED_RADIUS 5

using namespace std;

extern IVEngineServer *Engine;
extern IBotManager *IIBotManager;
extern IPlayerInfoManager *IIPlayerInfoManager;
extern IServerPluginHelpers *IIServerPluginHelpers;
extern WaypointManager *_WaypointManager;
extern IServerGameClients *IIServerGameClients;

const char *Name;
edict_t *_Edict; // Not const for convenience reasons
IBotController *_IIBotController;
IPlayerInfo *_IIPlayerInfo;
BotHelper *_BotHelper;

TFClass _CurrentClass;
Vector _LastPos;
uint8_t _PosStuckTime;
stack<WaypointNode*> _WaypointNodeStack;
int _PressedButtons;
WaypointNode *_ClosestWaypointNode;

Bot::Bot(edict_t *edict, const char *name) : Name(name), _Edict(edict),
	_IIBotController(IIBotManager->GetBotController(edict)),
	_IIPlayerInfo(IIPlayerInfoManager->GetPlayerInfo(edict)),
	_BotHelper(new BotHelper(this)) {
	_IIPlayerInfo->ChangeTeam(2);
	_BotHelper->RandomClass();
	_ResetState();
}

Bot::~Bot() {
	delete _BotHelper;
}

void Bot::Think() {
	_PressedButtons = 0;

	Vector currentPos = GetPos();
	if (Util::DistanceToNoZ(currentPos, _LastPos) < POS_STUCK_RADIUS) {
		_PosStuckTime++;
		if (_PosStuckTime > POS_STUCK_GIVEUPTIME) {
			_PosStuckTime = 0;
			_UpdateNewWaypointNodeStack();
		} else if (_PosStuckTime > POS_STUCK_STARTPANICTIME) {
			_PressedButtons |= IN_JUMP;
			_PressedButtons |= IN_DUCK;
		}
	}
	else {
		_PosStuckTime = 0;
		_LastPos = currentPos;
		_UpdateClosestWaypointNode();
	}

	QAngle lookAtPos;
	Vector2D movement;

	if (_WaypointNodeStack.empty())
		_UpdateNewWaypointNodeStack();
	else {
		Vector nodePos = _WaypointNodeStack.top()->Pos;
		if (currentPos.DistTo(nodePos) <= WAYPOINTNODE_TOUCHED_RADIUS)
			_WaypointNodeStack.pop();
		else
			movement = _BotHelper->GetIdealMoveSpeedsToPos(nodePos);
		Vector currentEarPos;
		IIServerGameClients->ClientEarPosition(_Edict, &currentEarPos);
		// Look at normal height instead of onto node directly
		lookAtPos = _BotHelper->GetLookAtAngleForPos(
			Vector(nodePos.x, nodePos.y, nodePos.z + currentEarPos.z - currentPos.z));
	}

	_IIBotController->RunPlayerMove(
		&_BotHelper->ConstructBotCmd(lookAtPos, movement, _PressedButtons));
}

edict_t *Bot::GetEdict() const {
	return _Edict;
}

bool Bot::Exists() const {
	return _IIPlayerInfo->IsConnected();
}

Vector Bot::GetPos() const {
	return _IIPlayerInfo->GetAbsOrigin();
}

Vector Bot::GetEarPos() const {
	Vector earPos;
	IIServerGameClients->ClientEarPosition(_Edict, &earPos);
	return earPos;
}

QAngle Bot::GetAngle() const {
	return _IIPlayerInfo->GetAbsAngles();
}

TFClass Bot::GetClass() const {
	return _CurrentClass;
}

void Bot::ChangeClass(TFClass tfClass) {
	char newClass[16];
	_BotHelper->TFClassToJoinName(tfClass, newClass);
	char cmd[32];
	sprintf_s(cmd, "joinclass %s", newClass);
	IIServerPluginHelpers->ClientCommand(_Edict, cmd);
	_CurrentClass = tfClass;
}

void Bot::_ResetState() {
	_PressedButtons = 0;
	_ClosestWaypointNode = nullptr;
}

void Bot::_UpdateNewWaypointNodeStack() {
	if (!_ClosestWaypointNode)
		_UpdateClosestWaypointNode();
	// If still nullptr, no waypoint nodes exist
	if (_ClosestWaypointNode) {
		if (!_WaypointNodeStack.empty())
			_WaypointNodeStack = stack<WaypointNode*>();
		_WaypointManager->GetWaypointNodeQueueToTargetNode(_ClosestWaypointNode,
			_WaypointManager->GetRandomWaypointNode(), &_WaypointNodeStack);
	}
}

void Bot::_UpdateClosestWaypointNode() {
	_ClosestWaypointNode = _WaypointManager->GetClosestWaypointNode(_LastPos);
}