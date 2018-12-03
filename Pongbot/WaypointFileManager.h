#pragma once
#include "ConVarBase.h"
#include <iostream>
#include <vector>

using namespace std;

class WaypointNode;

class WaypointFileManager : public ConVarBase {
public:
	static void Init(vector<WaypointNode*> *waypointNodes);
	static void Destroy();
private:
	WaypointFileManager(vector<WaypointNode*> *waypointNodes);
public:
	void Read();
	void Write();
private:
	char _CurrentMapName[32];
	vector<WaypointNode*> *_WaypointNodes;
private:
	bool _CheckDir(char *fileName);
// Hooks
private:
	bool _OnLevelInit(const char *pMapName, char const *pMapEntities, char const *pOldLevel,
		char const *pLandmarkName, bool loadGame, bool background);
};

extern WaypointFileManager *_WaypointFileManager;