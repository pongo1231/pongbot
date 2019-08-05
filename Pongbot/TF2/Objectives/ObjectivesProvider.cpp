#include <stdafx.h>
#include "ObjectivesProvider.h"
#include "../../Bot/Bot.h"
#include "../Entity/EntityProvider.h"
#include "../../Util.h"
#include "CTFFlagStatusType.h"
#include "../TFTeam.h"
#include "../../Bot/BotManager.h"
#include "../../ConVarHolder.h"
#include "../Entity/CTFFlag.h"
#include <metamod/ISmmAPI.h>

extern IVEngineServer* Engine;

ObjectivesProvider* _ObjectivesProvider = nullptr;

void ObjectivesProvider::Init()
{
	if (!_ObjectivesProvider)
	{
		Util::DebugLog("INIT ObjectivesProvider");

		_ObjectivesProvider = new ObjectivesProvider();
	}
}

void ObjectivesProvider::Destroy()
{
	if (_ObjectivesProvider)
	{
		Util::DebugLog("DESTROY ObjectivesProvider");

		delete _ObjectivesProvider;
		_ObjectivesProvider = nullptr;
	}
}

std::vector<Objective> ObjectivesProvider::GetBotPushObjectives(Bot* bot) const
{
	return bot->GetTeam() == TEAM_RED ? _BlueObjectives : _RedObjectives;
}

std::vector<Objective> ObjectivesProvider::GetBotDefendObjectives(Bot* bot) const
{
	return bot->GetTeam() == TEAM_RED ? _RedObjectives : _BlueObjectives;
}

std::vector<Objective> ObjectivesProvider::GetAllObjectives() const
{
	// Merge both objective lists
	std::vector<Objective> objectives = _RedObjectives;
	for (Objective objective : _BlueObjectives)
	{
		objectives.push_back(objective);
	}

	return objectives;
}

void ObjectivesProvider::OnGameFrame()
{
	if (!_BotManager->BotsInGame())
	{
		return;
	}

	static float tickTime;
	float engineTime = Engine->Time();
	if (tickTime > engineTime)
	{
		return;
	}
	tickTime = engineTime + _ConVarHolder->CVarObjectiveUpdateTick->GetFloat();

	_RedObjectives.clear();
	_BlueObjectives.clear();
	_UpdateCTFObjectives();
}

void ObjectivesProvider::_UpdateCTFObjectives()
{
	std::vector<Entity> itemFlags = _EntityProvider->SearchEntitiesByClassname("item_teamflag");
	for (Entity entity : itemFlags)
	{
		CTFFlag entityFlag(entity);

		Objective objective(entityFlag.GetEdict(), ITEMFLAG, entityFlag.GetPos(), entityFlag.GetStatus());
		if (entityFlag.GetTeam() == TEAM_RED)
		{
			_RedObjectives.push_back(objective);
		}
		else
		{
			_BlueObjectives.push_back(objective);
		}
	}
}