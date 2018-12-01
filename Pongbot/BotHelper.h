#pragma once
#include <hlsdk/public/mathlib/mathlib.h>

class Bot;

class BotHelper {
public:
	BotHelper(Bot *bot);
	QAngle GetLookAtAngle(Vector lookAtPos);
	void GetIdealMoveSpeeds(Vector targetPos, vec_t &forth, vec_t &side);
private:
	Bot *_Bot;
private:
	vec_t _CorrectAngle(vec_t angle);
	vec_t _GetYawAngle(Vector targetPos);
};

