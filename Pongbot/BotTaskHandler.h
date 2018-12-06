#pragma once
#include <hlsdk/public/mathlib/mathlib.h>

class Bot;
class BotTask;

class BotTaskHandler {
public:
	BotTaskHandler(Bot *bot);
	~BotTaskHandler();

	void OnThink(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt);

private:
	Bot *_ABot; // Avoid conflicts with BotTask's _Bot
	BotTask *_BotTask;
};