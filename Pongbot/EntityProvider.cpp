#include "EntityProvider.h"
#include "EntityDataProvider.h"
#include "BotManager.h"
#include "ConVarHolder.h"
#include "Entity.h"
#include <metamod/ISmmAPI.h>

extern IVEngineServer *Engine;

EntityProvider *_EntityProvider;

std::vector<Entity> _Entities;

void EntityProvider::Init()
{
	if (!_EntityProvider)
	{
		EntityDataProvider::Init();
		_EntityProvider = new EntityProvider();
	}
}

void EntityProvider::Destroy()
{
	if (_EntityProvider)
	{
		EntityDataProvider::Destroy();
		delete _EntityProvider;
	}
}

std::vector<Entity> EntityProvider::GetEntities() const
{
	return _Entities;
}

std::vector<Entity> EntityProvider::SearchEntitiesByClassname(const char *classname) const
{
	std::vector<Entity> foundEntities;
	for (Entity entity : _Entities)
		if (strcmp(entity.GetEdict()->GetClassName(), classname) == 0)
			foundEntities.push_back(entity);

	return foundEntities;
}

void EntityProvider::OnGameFrame()
{
	if (!_BotManager->BotsInGame())
		return;

	static float tickTime;
	float currentTime = Engine->Time();
	if (tickTime > currentTime)
		return;
	tickTime = currentTime + _ConVarHolder->CVarEntityProviderTick->GetFloat();

	_Entities.clear();
	for (int i = 1; i < Engine->GetEntityCount(); i++)
	{
		Entity entity(Engine->PEntityOfEntIndex(i));
		if (entity.Exists())
		{
			bool isPlayer = entity.IsPlayer();
			if (!isPlayer || (isPlayer && entity.GetTeam() != TEAM_SPEC && !entity.IsDead()))
				_Entities.push_back(entity);
		}
	}
}