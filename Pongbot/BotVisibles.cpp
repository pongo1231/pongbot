#include "BotVisibles.h"
#include "Bot.h"
#include "BotVisiblesProvider.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/iserverentity.h>
#include <vector>

extern IVEngineServer *Engine;
extern BotVisiblesProvider *_BotVisiblesProvider;

Bot *_MBot;

BotVisibles::BotVisibles(Bot *bot) : _MBot(bot) {}

void BotVisibles::OnGameFrame() {
	static float waitTime;
	float currentTime = Engine->Time();
	if (waitTime > currentTime)
		return;
	waitTime = currentTime + 10;

	for (CBaseEntity *entity : _BotVisiblesProvider->GetEntities()) {
		//entity->a();
	}
}