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

int ObjectivesProvider::GetRoundTimerStatus() const
{
	return _RoundTimer.GetRoundTimerStatus();
}

bool ObjectivesProvider::IsRoundActive() const
{
	return _RoundActive;
}

void ObjectivesProvider::OnGameFrame()
{
	if (!_BotManager->BotsInGame())
	{
		return;
	}

	static float tickTime = 0.f;
	float engineTime = Engine->Time();
	if (tickTime > engineTime)
	{
		return;
	}
	tickTime = engineTime + _ConVarHolder->CVarObjectiveUpdateTick->GetFloat();

	_UpdateCTFObjectives();
}

void ObjectivesProvider::OnLevelInit(const char* pMapName, char const* pMapEntities,
		char const* pOldLevel, char const* pLandmarkName, bool loadGame, bool background)
{
	_UpdateRoundTimer();
}

void ObjectivesProvider::OnRoundStart()
{
	Util::DebugLog("RoundActive = false");
	_RoundActive = false;
}

void ObjectivesProvider::OnRoundActive()
{
	Util::DebugLog("RoundActive = true");
	_RoundActive = true;
}

void ObjectivesProvider::_UpdateCTFObjectives()
{
	_RedObjectives.clear();
	_BlueObjectives.clear();

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

void ObjectivesProvider::_UpdateRoundTimer()
{
	std::vector<Entity> roundTimers = _EntityProvider->SearchEntitiesByClassname("team_round_timer");
	Util::DebugLog("RoundTimers: %d", roundTimers.size());
	if (!roundTimers.empty())
	{
		_RoundTimer = roundTimers[0];
	}
}