#pragma once
#include "GameFramable.h"

class Bot;

class BotVisibles : public GameFramable {
public:
	BotVisibles(Bot *bot);

public:
	void OnGameFrame();

private:
	Bot *_MBot;
};