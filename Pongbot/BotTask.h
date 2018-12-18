#pragma once
#include "Bot.h"
#include <hlsdk/public/mathlib/mathlib.h>
#include <hlsdk/game/shared/in_buttons.h>

class BotTask
{
public:
	BotTask(Bot *bot);

public:
	virtual void OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt) = 0;

protected:
	Bot *_GetBot() const;

private:
	Bot *_Bot;
};