#include "BotTask.h"

Bot *_Bot;

BotTask::BotTask(Bot *bot) : _Bot(bot) {}

QAngle BotTask::_GetLookAtAngleForPos(Vector lookAtPos) {
	Vector vectorAngle = lookAtPos - _Bot->GetEarPos();
	QAngle angle;
	VectorAngles(vectorAngle / vectorAngle.Length(), angle);
	return angle;
}

Vector2D BotTask::_GetIdealMoveSpeedsToPos(Vector targetPos) {
	Vector2D sins;
	SinCos(DEG2RAD(_GetYawAngle(targetPos)), &sins.y, &sins.x);
	sins = sins / sins.Length() * TF2Helper::GetClassSpeed(_Bot->GetClass()) * 3;
	return sins;
}

vec_t BotTask::_CorrectAngle(vec_t angle) {
	if (angle > 180)
		angle -= 360;
	else if (angle < -180)
		angle += 360;
	return angle;
}

vec_t BotTask::_GetYawAngle(Vector targetPos) {
	return _CorrectAngle(_Bot->GetAngle().y - _CorrectAngle(_GetLookAtAngleForPos(targetPos).y));
}