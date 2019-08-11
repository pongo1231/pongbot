#pragma once

class IEventHooker
{
public:
	IEventHooker();
	~IEventHooker();

public:
	virtual void OnObjectiveUpdate() {}
	virtual void OnLevelInit(const char* pMapName, char const* pMapEntities,
		char const* pOldLevel, char const* pLandmarkName, bool loadGame, bool background) {}
	virtual void OnRoundStart() {}
	virtual void OnRoundActive() {}
};