#include "EntityProvider.h"
#include "EntityDataProvider.h"
#include "BotManager.h"
#include "ConVarHolder.h"
#include <metamod/ISmmAPI.h>

extern IVEngineServer *Engine;

EntityProvider *_EntityProvider;

std::vector<edict_t*> _Edicts;

EntityProvider::EntityProvider()
{}

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

std::vector<edict_t*> EntityProvider::GetEdicts() const
{
	return _Edicts;
}

std::vector<edict_t*> EntityProvider::SearchEdictsByClassname(const char *classname) const
{
	std::vector<edict_t*> foundEdicts;
	for (edict_t *edict : _Edicts)
		if (strcmp(edict->GetClassName(), classname) == 0)
			foundEdicts.push_back(edict);
	return foundEdicts;
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

	_Edicts.clear();
	for (int i = 1; i < Engine->GetEntityCount(); i++)
	{
		edict_t *edict = Engine->PEntityOfEntIndex(i);
		if (edict)
			_Edicts.push_back(edict);
	}
}