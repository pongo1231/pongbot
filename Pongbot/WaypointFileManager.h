#pragma once
#include "ConVarBase.h"
#include <iostream>
#include <vector>

class WaypointNode;

class WaypointFileManager : public ConVarBase {
private:
	WaypointFileManager();

public:
	static void Init(std::vector<WaypointNode*> *waypointNodes);
	static void Destroy();

	void Read();
	void Write();

private:
	char _CurrentMapName[32];

	bool _CheckDir(char *fileName);
	bool _OnLevelInit(const char *pMapName, char const *pMapEntities, char const *pOldLevel,
		char const *pLandmarkName, bool loadGame, bool background);
};

extern WaypointFileManager *_WaypointFileManager;