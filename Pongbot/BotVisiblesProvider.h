#pragma once
#include "IGameFramable.h"
#include <hlsdk/public/edict.h>
#include <vector>

class BotVisiblesProvider : public IGameFramable
{
private:
	BotVisiblesProvider();

public:
	static void Init();
	static void Destroy();

	std::vector<edict_t*> GetVisibleEdicts() const;
	void OnGameFrame();

private:
	std::vector<edict_t*> _Edicts;

	bool _IsEdictRelevant(edict_t *edict);
};

extern BotVisiblesProvider *_BotVisiblesProvider;