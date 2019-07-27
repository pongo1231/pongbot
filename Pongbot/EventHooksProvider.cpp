#include "stdafx.h"
#include "EventHooksProvider.h"
#include "IEventHooker.h"
#include "BotManager.h"
#include "ConVarHolder.h"
#include "Util.h"
#include <metamod/ISmmAPI.h>

extern IVEngineServer* Engine;

EventHooksProvider* _EventHooksProvider;

void EventHooksProvider::Init()
{
	if (!_EventHooksProvider)
	{
		Util::DebugLog("INIT EventHooksProvider");

		_EventHooksProvider = new EventHooksProvider();
	}
}

void EventHooksProvider::Destroy()
{
	if (_EventHooksProvider)
	{
		Util::DebugLog("DESTROY EventHooksProvider");

		delete _EventHooksProvider;
	}
}

void EventHooksProvider::AddEventHooker(IEventHooker* eventHooker)
{
	// Check if list doesn't contain it first
	for (IEventHooker* listEventHooker : _EventHookers)
	{
		if (listEventHooker == eventHooker)
		{
			return;
		}
	}

	_EventHookers.push_back(eventHooker);
}

void EventHooksProvider::RemoveEventHooker(IEventHooker* eventHooker)
{
	for (unsigned int i = 0; i < _EventHookers.size(); i++)
	{
		if (_EventHookers[i] == eventHooker)
		{
			_EventHookers.erase(_EventHookers.begin() + i);
			return;
		}
	}
}

void EventHooksProvider::OnGameFrame()
{
	if (!_BotManager->BotsInGame())
	{
		return;
	}

	// Timer might make it less accurate for some events
	// So remove if an event needs to be accurate
	static float tickTime;
	float currentTime = Engine->Time();
	if (tickTime > currentTime)
	{
		return;
	}
	tickTime = currentTime + _ConVarHolder->CVarEventProviderTick->GetFloat();

	_CheckObjectiveUpdates();
}

void EventHooksProvider::_CheckObjectiveUpdates()
{
	std::vector<Objective> newObjectives = _ObjectivesProvider->GetAllObjectives();

	// Compare new objectives with previous ones
	for (Objective newObjective : newObjectives)
	{
		for (Objective prevObjective : _PrevObjectives)
		{
			// Either same edict (e.g. itemflags) or same pos (capture points) means it's the same objective
			if (((newObjective.Edict && newObjective.Edict == prevObjective.Edict)
				|| newObjective.Pos == prevObjective.Pos) && newObjective.Status != prevObjective.Status)
			{
				// Status changed
				for (IEventHooker* eventHooker : _EventHookers)
				{
					eventHooker->OnObjectiveUpdate();
				}
				break;
			}
		}
	}

	_PrevObjectives = newObjectives;
}