#include "BotVisiblesProvider.h"
#include "TFTeam.h"
#include "EntityProvider.h"
#include "Util.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

#define VISIBLES_PROVIDER_TICK 1

extern IVEngineServer *Engine;
extern IPlayerInfoManager *IIPlayerInfoManager;

BotVisiblesProvider *_BotVisiblesProvider;

std::vector<edict_t*> _VisibleEdicts;

void BotVisiblesProvider::Init()
{
	Assert(!_BotVisiblesProvider);
	_BotVisiblesProvider = new BotVisiblesProvider();
}

void BotVisiblesProvider::Destroy()
{
	Assert(_BotVisiblesProvider);
	delete _BotVisiblesProvider;
}

std::vector<edict_t*> BotVisiblesProvider::GetVisibleEdicts() const
{
	return _VisibleEdicts;
}

void BotVisiblesProvider::OnGameFrame()
{
	static float tickTime;
	float currentTime = Engine->Time();
	if (tickTime > currentTime)
		return;
	tickTime = currentTime + VISIBLES_PROVIDER_TICK;

	_VisibleEdicts.clear();
	for (edict_t *edict : _EntityProvider->GetEdicts())
		if (_IsEdictRelevant(edict))
			_VisibleEdicts.push_back(edict);
}

bool BotVisiblesProvider::_IsEdictRelevant(edict_t *edict)
{
	// TODO: more filters
	const char *className = edict->GetClassName();
	return (strcmp(className, "player") == 0 && !IIPlayerInfoManager->GetPlayerInfo(edict)->IsDead());
}