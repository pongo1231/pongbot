#include "WaypointFileManager.h"
#include "Util.h"
#include "WaypointNode.h"
#include <metamod/ISmmAPI.h>
#include <metamod/sourcehook.h>
#include <windows.h>
#include <fstream>

#define FILE_DIR "tf/addons/pongbot/waypoints/"
#define FILE_EXTENSION "pbw"

extern IServerGameDLL *Server;
extern SourceHook::ISourceHook *g_SHPtr;
extern PluginId g_PLID;

WaypointFileManager *_WaypointFileManager;

static std::vector<WaypointNode*> *_WaypointNodes;

char _CurrentMapName[32];

SH_DECL_HOOK6(IServerGameDLL, LevelInit, SH_NOATTRIB, 0, bool, char const *, char const *,
	char const *, char const *, bool, bool);

WaypointFileManager::WaypointFileManager()
{}

void WaypointFileManager::Init(std::vector<WaypointNode*> *waypointNodes)
{
	Assert(!_WaypointFileManager);

	_WaypointNodes = waypointNodes;

	_WaypointFileManager = new WaypointFileManager();

	SH_ADD_HOOK(IServerGameDLL, LevelInit, Server,
		SH_MEMBER(_WaypointFileManager, &WaypointFileManager::_OnLevelInit), true);
}

void WaypointFileManager::Destroy()
{
	Assert(_WaypointFileManager);

	SH_REMOVE_HOOK(IServerGameDLL, LevelInit, Server,
		SH_MEMBER(_WaypointFileManager, &WaypointFileManager::_OnLevelInit), true);

	delete _WaypointFileManager;
}

void WaypointFileManager::Read()
{
	char fileName[64];
	if (_CheckDir(fileName))
	{
		_WaypointNodes->clear();

		std::ifstream file(fileName);
		char fileBuffer[512];
		int connectedNodeIds[256][256];
		// Fill connectedNodeIds with -1 to not confuse with node id 0 instead
		memset(connectedNodeIds, -1, sizeof(connectedNodeIds));

		uint8_t i = 0;
		while (file.getline(fileBuffer, sizeof(fileBuffer)))
		{
			char lineBuffer[256][16];
			char *context = nullptr;
			uint8_t j = 0;
			char *token = strtok_s(fileBuffer, ":", &context);
			do
				strcpy_s(lineBuffer[j++], token);
			while ((token = strtok_s(nullptr, ":", &context)) != nullptr);

			WaypointNode *node = new WaypointNode(atoi(lineBuffer[0]),
				Vector(atof(lineBuffer[1]), atof(lineBuffer[2]), atof(lineBuffer[3])));
			node->Flags = atoi(lineBuffer[4]);
			_WaypointNodes->push_back(node);

			// Store connected node ids for reference
			for (uint8_t k = 5; k < 256; k++)
			{
				char *content = lineBuffer[k];
				if (strcmp(content, "\\") == 0)
					break;
				else
					connectedNodeIds[i][k - 5] = atoi(content);
			}

			i++;
		}

		// Now handle the node connections
		for (i = 0; i < _WaypointNodes->size(); i++)
			for (uint8_t j = 0; j < 256; j++)
				if (connectedNodeIds[i][j] == -1)
					break;
				else
					for (WaypointNode *nodeToConnect : *_WaypointNodes)
						if (nodeToConnect->Id == connectedNodeIds[i][j])
							(*_WaypointNodes)[i]->ConnectToNode(nodeToConnect);

		file.close();
		Util::Log("Waypoint loaded!");
	}
}

void WaypointFileManager::Write()
{
	char fileName[64];
	if (_CheckDir(fileName))
	{
		std::ofstream file(fileName);
		// Write all nodes to file
		for (WaypointNode *node : *_WaypointNodes)
		{
			Vector pos = node->Pos;
			file << (int) node->Id << ":" << pos.x << ":" << pos.y << ":" << pos.z << ":" << node->Flags;

			// Also write IDs of saved nodes to file too
			for (WaypointNode *connectedNode : node->GetConnectedNodes())
				file << ":" << connectedNode->Id;

			file << "\\" << std::endl;
		}

		file.close();
		Util::Log("Waypoint saved!");
	}
}

bool WaypointFileManager::_CheckDir(char *fileName) {
	struct stat info;
	stat(FILE_DIR, &info);
	if (~info.st_mode & S_IFDIR)
	{
		// Create dir
		bool created = CreateDirectory(FILE_DIR, nullptr);
		if (!created)
		{
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
	char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background)
{
	strcpy(_CurrentMapName, pMapName);
	_WaypointFileManager->Read();

	return true;
}

CON_COMMAND(pongbot_waypoint_save, "Saves current waypoint to file")
{
	_WaypointFileManager->Write();
}

CON_COMMAND(pongbot_waypoint_load, "Loads waypoint from file")
{
	_WaypointFileManager->Read();
}