#include "WaypointFileManager.h"
#include "Util.h"
#include "WaypointNode.h"
#include <metamod/ISmmAPI.h>
#include <metamod/sourcehook.h>
#include <windows.h>
#include <iostream>
#include <fstream>

#define FILE_DIR "tf/addons/pongbot/waypoints/"
#define FILE_EXTENSION "pbw"

extern IServerGameDLL *Server;
extern SourceHook::ISourceHook *g_SHPtr;
extern PluginId g_PLID;

WaypointFileManager *_WaypointFileManager;

char _CurrentMapName[32];
vector<WaypointNode*> *_WaypointNodes;

SH_DECL_HOOK6(IServerGameDLL, LevelInit, SH_NOATTRIB, 0, bool, char const *, char const *,
	char const *, char const *, bool, bool);

void WaypointFileManager::Init(vector<WaypointNode*> *waypointNodes) {
	Assert(!_WaypointFileManager);
	_WaypointFileManager = new WaypointFileManager(waypointNodes);
	SH_ADD_HOOK(IServerGameDLL, LevelInit, Server,
		SH_MEMBER(_WaypointFileManager, &WaypointFileManager::_OnLevelInit), true);
}

void WaypointFileManager::Destroy() {
	Assert(_WaypointFileManager);
	SH_REMOVE_HOOK(IServerGameDLL, LevelInit, Server,
		SH_MEMBER(_WaypointFileManager, &WaypointFileManager::_OnLevelInit), true);
	delete _WaypointFileManager;
}

WaypointFileManager::WaypointFileManager(vector<WaypointNode*> *waypointNodes)
	: _WaypointNodes(waypointNodes) {}

void WaypointFileManager::Read() {
	char fileName[64];
	if (_CheckDir(fileName)) {
		ifstream file(fileName);
		char buffer[512];
		istream *stream = &file.getline(buffer, sizeof(buffer));
		do {
			char lineBuffer[256][8];
			char *context = nullptr;
			char *token = strtok_s(buffer, ":", &context);
			unsigned int i = 0;
			do {
				strcpy_s(lineBuffer[i++], token);
			} while ((token = strtok_s(nullptr, ":", &context)) != nullptr);

			WaypointNode *node = new WaypointNode(atoi(lineBuffer[0]),
				Vector(atof(lineBuffer[1]), atof(lineBuffer[2]), atof(lineBuffer[3])));
			/*for (uint8_t i = 4; i < 256; i++) {
				char lineBuffer[]
			}*/
			_WaypointNodes->push_back(node);
		} while (stream = &stream->getline(buffer, sizeof(buffer)));
		file.close();
		Util::Log("Waypoint loaded!");
	}
}

void WaypointFileManager::Write() {
	char fileName[64];
	if (_CheckDir(fileName)) {
		ofstream file(fileName);
		// Write all nodes to file
		for (WaypointNode *node : *_WaypointNodes) {
			Vector pos = node->Pos;
			file << node->Id << ":" << pos.x << ":" << pos.y << ":"
				<< pos.z;
			// Also write IDs of saved nodes to file too
			for (WaypointNode *connectedNode : *node->GetConnectedNodes())
				// Don't write previously deleted nodes though
				if (connectedNode)
					file << ":" << connectedNode->Id;
			file << endl;
		}
		file.close();
		Util::Log("Waypoint saved!");
	}
}

bool WaypointFileManager::_CheckDir(char *fileName) {
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

	char _fileName[64];
	sprintf_s(_fileName, "%s%s.%s", FILE_DIR, _CurrentMapName, FILE_EXTENSION);
	strcpy(fileName, _fileName);
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