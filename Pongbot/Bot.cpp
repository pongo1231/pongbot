#include "Bot.h"
#include "BotManager.h"
#include "Util.h"
#include "BotHelper.h"
#include "WaypointManager.h"
#include <metamod/ISmmPlugin.h>
#include <hlsdk/game/shared/in_buttons.h>
#include <string>

#define POS_STUCK_RADIUS 0.1
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
WaypointNode *_TargetNode;

bool _IsShooting;
bool _IsJumping;
bool _IsCrouching;

Bot::Bot(edict_t *edict, const char *name) : Name(name), _Edict(edict),
	_IIBotController(IIBotManager->GetBotController(edict)),
	_IIPlayerInfo(IIPlayerInfoManager->GetPlayerInfo(edict)),
	_BotHelper(new BotHelper(this)) {
	_IIPlayerInfo->ChangeTeam(2);
	_ResetStates();
	_BotHelper->RandomClass();
}

Bot::~Bot() {
	delete _BotHelper;
}

void Bot::Think() {
	_ResetStates();

	Vector currentPos = _IIPlayerInfo->GetAbsOrigin();
	if (currentPos.DistTo(_LastPos) < POS_STUCK_RADIUS) {
		_PosStuckTime++;
		if (_PosStuckTime > 120) {
			_PosStuckTime = 0;
			_UpdateNewTargetNode();
		} else if (_PosStuckTime > 60) {
			_IsJumping = true;
			_IsCrouching = true;
		}
	} else {
		_PosStuckTime = 0;
		_LastPos = currentPos;
	}

	if (!_TargetNode)
		_UpdateNewTargetNode();
	else {
		Vector nodePos = _TargetNode->Pos;
		Vector currentEarPos;
		IIServerGameClients->ClientEarPosition(_Edict, &currentEarPos);
		if (currentPos.DistTo(nodePos) < WAYPOINTNODE_TOUCHED_RADIUS)
			_UpdateNewTargetNode();
		_IIBotController->RunPlayerMove(&_BotHelper->ConstructBotCmd(
			// Look at normal height instead of onto node directly
			_BotHelper->GetLookAtAngle(Vector(nodePos.x, nodePos.y, nodePos.z + currentEarPos.z - currentPos.z)),
			_BotHelper->GetIdealMoveSpeeds(nodePos),
			_ConstructButtonsState()
		));
	}
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

void Bot::_ResetStates() {
	_IsShooting = false;
	_IsJumping = false;
	_IsCrouching = false;
}

int Bot::_ConstructButtonsState() {
	int buttons = 0;
	if (_IsShooting)
		buttons |= IN_ATTACK;
	if (_IsJumping)
		buttons |= IN_JUMP;
	if (_IsCrouching)
		buttons |= IN_DUCK;
	buttons |= IN_SPEED;
	buttons |= IN_RUN;
	return buttons;
}

void Bot::_UpdateNewTargetNode() {
	_TargetNode = _WaypointManager->GetRandomWaypointNode();
}