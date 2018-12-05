#pragma once
#include <hlsdk/public/mathlib/mathlib.h>

class Bot;
class BotTask;

class BotTaskHandler {
public:
	BotTaskHandler(Bot *bot);
	~BotTaskHandler();

	void OnTick(int *pressedButtons, Vector2D *&movement, QAngle *&lookAt);

private:
	Bot *__Bot; // Avoid conflicts with BotTask's _Bot
	BotTask *_BotTask;
};