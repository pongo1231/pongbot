#include "BotVisiblesProvider.h"
#include "TFTeam.h"
#include "EntityProvider.h"
#include "Util.h"
#include "BotManager.h"
#include "ConVarHolder.h"
#include "Player.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/game/server/iplayerinfo.h>

extern IVEngineServer *Engine;
extern IPlayerInfoManager *IIPlayerInfoManager;

BotVisiblesProvider *_BotVisiblesProvider;

std::vector<Entity> _VisibleEntities;

void BotVisiblesProvider::Init()
{
	if (!_BotVisiblesProvider)
	{
		Util::DebugLog("INIT BotVisiblesProvider");

		_BotVisiblesProvider = new BotVisiblesProvider();
	}
}

void BotVisiblesProvider::Destroy()
{
	if (_BotVisiblesProvider)
	{
		Util::DebugLog("DESTROY BotVisiblesProvider");

		delete _BotVisiblesProvider;
	}
}

std::vector<Entity> BotVisiblesProvider::GetVisibleEntities() const
{
	return _VisibleEntities;
}

void BotVisiblesProvider::OnGameFrame()
{
	if (!_BotManager->BotsInGame())
		return;

	static float tickTime;
	float currentTime = Engine->Time();
	if (tickTime > currentTime)
		return;
	tickTime = currentTime + _ConVarHolder->CVarVisiblesProviderTick->GetFloat();

	_VisibleEntities.clear();
	for (Entity entity : _EntityProvider->GetEntities())
		if (_IsEntityRelevant(entity))
			_VisibleEntities.push_back(entity);
}

bool BotVisiblesProvider::_IsEntityRelevant(Entity entity) const
{
	// TODO: more filters
	const char *className = entity.GetEdict()->GetClassName();
	return entity.IsPlayer() || strcmp(className, "obj_sentrygun") == 0 || strcmp(className, "obj_dispenser") == 0;
}