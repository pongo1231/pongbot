#include "BotVisibles.h"
#include "Bot.h"
#include "BotVisiblesProvider.h"
#include "Util.h"
#include <metamod/ISmmAPI.h>

extern IVEngineServer *Engine;
extern BotVisiblesProvider *_BotVisiblesProvider;

Bot *_MBot;
std::vector<edict_t*> _VisibleEdicts;

BotVisibles::BotVisibles(Bot *bot) : _MBot(bot) {}

std::vector<edict_t*> BotVisibles::GetVisibleEdicts() const {
	return _VisibleEdicts;
}

void BotVisibles::OnGameFrame() {
	static float waitTime;
	float currentTime = Engine->Time();
	if (waitTime > currentTime)
		return;
	waitTime = currentTime + 0.2;

	_VisibleEdicts.clear();

	edict_t *botEdict = _MBot->GetEdict();
	for (edict_t *edict : _BotVisiblesProvider->GetAllEdicts()) {
		// TODO: raycasting and shit
		if (edict != botEdict)
			_VisibleEdicts.push_back(edict);
	}
}