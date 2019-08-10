#pragma once
#include "../IConVarBase.h"
#include "../TF2/Events/IEventHooker.h"
#include <vector>

class WaypointNode;

class WaypointFileManager : public IConVarBase, IEventHooker
{
private:
	WaypointFileManager() {}

public:
	static void Init(std::vector<WaypointNode*>* waypointNodes);
	static void Destroy();

	void Read();
	void Write();

	virtual void OnLevelInit(const char* pMapName, char const* pMapEntities, char const* pOldLevel,
		char const* pLandmarkName, bool loadGame, bool background);

private:
	char _CurrentMapName[32];

	bool _CheckDir(char* fileName);
	bool _OnLevelInit(const char* pMapName);
};

extern WaypointFileManager* _WaypointFileManager;