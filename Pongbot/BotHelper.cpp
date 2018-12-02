#include "BotHelper.h"
#include "Bot.h"
#include "Util.h"
#include "TF2Helper.h"
#include <hlsdk/public/mathlib/mathlib.h>
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

const Bot *_Bot;

BotHelper::BotHelper(Bot *bot) : _Bot(bot) {}

QAngle BotHelper::GetLookAtAngleForPos(Vector lookAtPos) {
	Vector vectorAngle = lookAtPos - _Bot->GetEarPos();
	QAngle angle;
	VectorAngles(vectorAngle / vectorAngle.Length(), angle);
	return angle;
}

Vector2D BotHelper::GetIdealMoveSpeedsToPos(Vector targetPos) {
	Vector2D sins;
	SinCos(DEG2RAD(_GetYawAngle(targetPos)), &sins.y, &sins.x);
	sins = sins / sins.Length() * TF2Helper::GetClassSpeed(_Bot->GetClass()) * 3;
	return sins;
}

void BotHelper::TFClassToJoinName(TFClass tfClass, char *tfClassName) {
	switch (tfClass) {
	case SCOUT:
		strcpy(tfClassName, "scout");
		break;
	case SOLDIER:
		strcpy(tfClassName, "soldier");
		break;
	case PYRO:
		strcpy(tfClassName, "pyro");
		break;
	case DEMO:
		strcpy(tfClassName, "demoman");
		break;
	case HEAVY:
		strcpy(tfClassName, "heavyweapons");
		break;
	case ENGI:
		strcpy(tfClassName, "engineer");
		break;
	case MED:
		strcpy(tfClassName, "medic");
		break;
	case SNIPER:
		strcpy(tfClassName, "sniper");
		break;
	case SPY:
		strcpy(tfClassName, "spy");
		break;
	}
}

CBotCmd BotHelper::ConstructBotCmd(QAngle viewAngle, Vector2D movement, int buttons) {
	CBotCmd cmd;
	cmd.viewangles = viewAngle;
	cmd.forwardmove = movement.x;
	cmd.sidemove = movement.y;
	cmd.buttons = buttons;
	return cmd;
}

void BotHelper::RandomClass() {
	_Bot->ChangeClass(TFClass(Util::RandomInt(0, 8)));
}

vec_t BotHelper::_CorrectAngle(vec_t angle) {
	if (angle > 180)
		angle -= 360;
	else if (angle < -180)
		angle += 360;
	return angle;
}

vec_t BotHelper::_GetYawAngle(Vector targetPos) {
	QAngle newAngle = GetLookAtAngleForPos(targetPos);
	return _CorrectAngle(_Bot->GetAngle().y - _CorrectAngle(newAngle.y));
}