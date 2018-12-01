#include "BotHelper.h"
#include "Bot.h"
#include "Util.h"

const Bot *_Bot;

BotHelper::BotHelper(Bot *bot) : _Bot(bot) {}

QAngle BotHelper::GetLookAtAngle(Vector lookAtPos) {
	Vector vectorAngle = lookAtPos - _Bot->GetOrigin();
	QAngle angle;
	VectorAngles(vectorAngle / vectorAngle.Length(), angle);
	return angle;
}

void BotHelper::GetIdealMoveSpeeds(Vector targetPos, vec_t &forth, vec_t &side) {
	Vector2D sins;
	SinCos(DEG2RAD(_GetYawAngle(targetPos)), &sins.y, &sins.x);
	sins = sins / sins.Length() * TF2Helper::GetClassSpeed(_Bot->GetClass());
	forth = sins.x;
	side = sins.y;
}

vec_t BotHelper::_CorrectAngle(vec_t angle) {
	if (angle > 180)
		angle -= 360;
	else if (angle < -180)
		angle += 360;
	return angle;
}

vec_t BotHelper::_GetYawAngle(Vector targetPos) {
	QAngle newAngle = GetLookAtAngle(targetPos);
	return _CorrectAngle(_Bot->GetAngles().y - _CorrectAngle(newAngle.y));
}