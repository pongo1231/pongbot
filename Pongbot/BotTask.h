#pragma once
#include "Bot.h"
#include <hlsdk/public/mathlib/mathlib.h>
#include <hlsdk/game/shared/in_buttons.h>

class BotTask {
public:
	BotTask(Bot *bot);

	virtual void OnTick(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt) = 0;

protected:
	Bot *_Bot;

	QAngle _GetLookAtAngleForPos(Vector lookAtPos);
	Vector2D _GetIdealMoveSpeedsToPos(Vector targetPos);

private:
	vec_t _CorrectAngle(vec_t angle);
	vec_t _GetYawAngle(Vector targetPos);
};