#include "ObjectivesProvider.h"
#include "Bot.h"
#include "EntityProvider.h"
#include "EntityDataProvider.h"
#include "Util.h"
#include "CTFFlagStatusType.h"
#include "TFTeam.h"
#include <metamod/ISmmAPI.h>

#define OBJECTIVE_UPDATE_TICK 0.5f

extern IVEngineServer *Engine;

ObjectivesProvider *_ObjectivesProvider;

std::vector<Objective> _RedObjectives;
std::vector<Objective> _BlueObjectives;

void ObjectivesProvider::Init()
{
	Assert(!_ObjectivesProvider);
	_ObjectivesProvider = new ObjectivesProvider();
}

void ObjectivesProvider::Destroy()
{
	Assert(_ObjectivesProvider);
	delete _ObjectivesProvider;
}

std::vector<Objective> ObjectivesProvider::GetBotPushObjectives(Bot *bot)
{
	return bot->GetTeam() == RED ? _BlueObjectives : _RedObjectives;
}

std::vector<Objective> ObjectivesProvider::GetBotDefendObjectives(Bot *bot)
{
	return bot->GetTeam() == RED ? _RedObjectives : _BlueObjectives;
}

void ObjectivesProvider::OnGameFrame()
{
	static float tickTime;
	float engineTime = Engine->Time();
	if (tickTime > engineTime)
		return;
	tickTime = engineTime + OBJECTIVE_UPDATE_TICK;

	_RedObjectives.clear();
	_BlueObjectives.clear();
	_UpdateCTFObjectives();
}

void ObjectivesProvider::_UpdateCTFObjectives()
{
	std::vector<edict_t*> itemFlags = _EntityProvider->SearchEdictsByClassname("item_teamflag");
	for (edict_t *itemFlag : itemFlags)
	{
		Objective objective(itemFlag, ITEMFLAG, Util::GetEdictOrigin(itemFlag),
			_EntityDataProvider->GetDataFromEdict<int>(itemFlag, FLAG_STATUS));
		if (_EntityDataProvider->GetDataFromEdict<TFTeam>(itemFlag, TEAM) == RED)
			_RedObjectives.push_back(objective);
		else
			_BlueObjectives.push_back(objective);
	}
}