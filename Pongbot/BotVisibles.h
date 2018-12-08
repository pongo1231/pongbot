#pragma once
#include "GameFramable.h"
#include <hlsdk/public/edict.h>
#include <vector>

class Bot;

class BotVisibles : public GameFramable {
public:
	BotVisibles(Bot *bot);

public:
	std::vector<edict_t*> GetVisibleEdicts() const;

	void OnGameFrame();

private:
	Bot *_MBot;
	std::vector<edict_t*> _VisibleEdicts;
};