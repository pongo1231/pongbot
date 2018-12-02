#pragma once
#include "ConVarBase.h"

class WaypointFileManager : public ConVarBase {
public:
	static void Init();
	static void Destroy();
private:
	WaypointFileManager();
public:
	void Read();
	void Write();
private:
	char _CurrentMapName[32];
private:
	bool _CheckFile();
// Hooks
private:
	bool _OnLevelInit(const char *pMapName, char const *pMapEntities, char const *pOldLevel,
		char const *pLandmarkName, bool loadGame, bool background);
};

extern WaypointFileManager *_WaypointFileManager;