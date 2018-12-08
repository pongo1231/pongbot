#pragma once
#include "GameFramable.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/edict.h>
#include <vector>

class BotVisiblesProvider : public GameFramable {
private:
	BotVisiblesProvider();

public:
	static void Init();
	static void Destroy();

	std::vector<edict_t*> GetAllEdicts() const;
	void OnGameFrame();

private:
	std::vector<edict_t*> _Edicts;
};

extern BotVisiblesProvider *_BotVisiblesProvider;