#pragma once
#include "TF2Helper.h"
#include <hlsdk/public/mathlib/mathlib.h>
#include <hlsdk/public/edict.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

class Bot;

class BotHelper {
public:
	BotHelper(Bot *bot);
public:
	QAngle GetLookAtAngle(Vector lookAtPos);
	Vector2D GetIdealMoveSpeeds(Vector targetPos);
	void TFClassToJoinName(TFClass tfClass, char *tfClassName);
	CBotCmd ConstructBotCmd(QAngle viewAngle, Vector2D movement, int buttons);
	void RandomClass();
private:
	Bot *_Bot;
private:
	vec_t _CorrectAngle(vec_t angle);
	vec_t _GetYawAngle(Vector targetPos);
};

