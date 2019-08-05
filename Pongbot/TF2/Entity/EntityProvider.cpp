#include <stdafx.h>
#include "EntityProvider.h"
#include "EntityDataProvider.h"
#include "../../Bot/BotManager.h"
#include "../../ConVarHolder.h"
#include "Player.h"
#include <metamod/ISmmAPI.h>

extern IVEngineServer* Engine;

EntityProvider* _EntityProvider = nullptr;

void EntityProvider::Init()
{
	if (!_EntityProvider)
	{
		Util::DebugLog("INIT EntityProvider");

		EntityDataProvider::Init();
		_EntityProvider = new EntityProvider();
	}
}

void EntityProvider::Destroy()
{
	if (_EntityProvider)
	{
		Util::DebugLog("DESTROY EntityProvider");

		EntityDataProvider::Destroy();
		delete _EntityProvider;
		_EntityProvider = nullptr;
	}
}

std::vector<Entity> EntityProvider::GetEntities() const
{
	return _Entities;
}

std::vector<Entity> EntityProvider::SearchEntitiesByClassname(const char* classname) const
{
	std::vector<Entity> foundEntities;
	for (Entity entity : _Entities)
	{
		if (entity.Exists() && strcmp(entity.GetEdictClassName(), classname) == 0)
		{
			foundEntities.push_back(entity);
		}
	}

	return foundEntities;
}

void EntityProvider::OnGameFrame()
{
	if (!_BotManager->BotsInGame())
	{
		return;
	}

	static float tickTime;
	float currentTime = Engine->Time();
	if (tickTime > currentTime)
	{
		return;
	}
	tickTime = currentTime + _ConVarHolder->CVarEntityProviderTick->GetFloat();

	_Entities.clear();
	for (int i = 1; i < Engine->GetEntityCount(); i++)
	{
		Entity entity(Engine->PEntityOfEntIndex(i));
		if (entity.Exists())
		{
			if (!entity.IsPlayer() || (entity.GetTeam() != TEAM_SPEC && !Player(entity).IsDead()))
			{
				_Entities.push_back(entity);
			}
		}
	}
}