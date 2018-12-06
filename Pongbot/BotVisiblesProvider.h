#pragma once
#include "GameFramable.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/iserverunknown.h>
#include <vector>

class BotVisiblesProvider : public GameFramable {
private:
	BotVisiblesProvider();

public:
	static void Init();
	static void Destroy();

	std::vector<CBaseEntity*> GetEntities();
	void OnGameFrame();

private:
	std::vector<CBaseEntity*> _Entities;
};

extern BotVisiblesProvider *_BotVisiblesProvider;