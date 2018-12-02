#include "WaypointFileManager.h"
#include "Util.h"
#include <metamod/ISmmAPI.h>
#include <metamod/sourcehook.h>
#include <windows.h>
#include <iostream>
#include <fstream>

#define FILE_DIR "tf/addons/pongbot/waypoints/"
#define FILE_EXTENSION "pbw"

using namespace std;

extern IServerGameDLL *Server;
extern SourceHook::ISourceHook *g_SHPtr;
extern PluginId g_PLID;

WaypointFileManager *_WaypointFileManager;

char _CurrentMapName[32];

SH_DECL_HOOK6(IServerGameDLL, LevelInit, SH_NOATTRIB, 0, bool, char const *, char const *,
	char const *, char const *, bool, bool);

void WaypointFileManager::Init() {
	Assert(!_WaypointFileManager);
	_WaypointFileManager = new WaypointFileManager();
	SH_ADD_HOOK(IServerGameDLL, LevelInit, Server,
		SH_MEMBER(_WaypointFileManager, &WaypointFileManager::_OnLevelInit), true);
}

void WaypointFileManager::Destroy() {
	Assert(_WaypointFileManager);
	SH_REMOVE_HOOK(IServerGameDLL, LevelInit, Server,
		SH_MEMBER(_WaypointFileManager, &WaypointFileManager::_OnLevelInit), true);
	delete _WaypointFileManager;
}

WaypointFileManager::WaypointFileManager() {}

void WaypointFileManager::Read() {
	if (_CheckFile())
		Util::Log("Waypoint loaded! (STUB!)");
}

void WaypointFileManager::Write() {
	if (_CheckFile())
		Util::Log("Waypoint saved! (STUB!)");
}

bool WaypointFileManager::_CheckFile() {
	// Check for dir first
	struct stat info;
	stat(FILE_DIR, &info);
	if (~info.st_mode & S_IFDIR) {
		// Create dir
		bool created = CreateDirectory(FILE_DIR, nullptr);
		if (!created) {
			Util::Log("Error while creating directory!");
			return false;
		}
	}

	// Now check for file
	char fileName[64];
	sprintf_s(fileName, "%s%s.%s", FILE_DIR, _CurrentMapName, FILE_EXTENSION);
	ofstream file(fileName);
	file.close();

	return true;
}

bool WaypointFileManager::_OnLevelInit(const char *pMapName, char const *pMapEntities,
	char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background) {
	strcpy(_CurrentMapName, pMapName);
	return true;
}

CON_COMMAND(pongbot_savewaypoint, "Saves current waypoint to file") {
	_WaypointFileManager->Write();
}

CON_COMMAND(pongbot_loadwaypoint, "Loads waypoint from file") {
	_WaypointFileManager->Read();
}