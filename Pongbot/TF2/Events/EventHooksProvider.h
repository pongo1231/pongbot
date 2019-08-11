#pragma once
#include "../../IGameFramable.h"
#include "../Objectives/ObjectivesProvider.h"
#include <hlsdk/public/igameevents.h>
#include <vector>

class IEventHooker;

class EventHooksProvider : public IGameFramable, public IGameEventListener2
{
private:
	EventHooksProvider() {}

public:
	static void Init();
	static void Destroy();

	void AddEventHooker(IEventHooker* eventHooker);
	void RemoveEventHooker(IEventHooker* eventHooker);

	virtual void OnGameFrame();
	virtual void FireGameEvent(IGameEvent* pEvent) {}

private:
	std::vector<IEventHooker*> _EventHookers;
	std::vector<Objective> _PrevObjectives;

	void _CheckObjectiveUpdates();
	bool _OnLevelInit(const char* pMapName, char const* pMapEntities,
		char const* pOldLevel, char const* pLandmarkName, bool loadGame, bool background) const;
	virtual bool _OnFireEvent(IGameEvent* pEvent, bool bDontBroadcast) const;
	virtual bool _OnFireEventPost(IGameEvent* pEvent, bool bDontBroadcast) const;
};

extern EventHooksProvider* _EventHooksProvider;