#include <stdafx.h>
#include "WaypointFileManager.h"
#include "../Util.h"
#include "WaypointNode.h"
#include <metamod/ISmmAPI.h>
#include <metamod/sourcehook/sourcehook.h>
#ifdef _WIN32
#include <windows.h>
#elif _LINUX
#include <sys/stat.h>
#endif
#include <fstream>
#include <stdint.h>

#define FILE_DIR "tf/addons/pongbot/waypoints/"
#define FILE_EXTENSION "pbw"

extern IServerGameDLL* Server;
extern SourceHook::ISourceHook* g_SHPtr;
extern PluginId g_PLID;

WaypointFileManager* _WaypointFileManager = nullptr;

static std::vector<WaypointNode*>* _WaypointNodes = nullptr;

SH_DECL_HOOK6(IServerGameDLL, LevelInit, SH_NOATTRIB, 0, bool, char const*, char const*,
	char const*, char const*, bool, bool);

void WaypointFileManager::Init(std::vector<WaypointNode*>* waypointNodes)
{
	if (!_WaypointFileManager)
	{
		Util::DebugLog("INIT WaypointFileManager");

		_WaypointNodes = waypointNodes;
		_WaypointFileManager = new WaypointFileManager();
		SH_ADD_HOOK(IServerGameDLL, LevelInit, Server, SH_MEMBER(_WaypointFileManager, &WaypointFileManager::_OnLevelInit), true);
	}
}

void WaypointFileManager::Destroy()
{
	if (_WaypointFileManager)
	{
		Util::DebugLog("DESTROY WaypointFileManager");

		SH_REMOVE_HOOK(IServerGameDLL, LevelInit, Server, SH_MEMBER(_WaypointFileManager, &WaypointFileManager::_OnLevelInit), true);
		delete _WaypointFileManager;
	}
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
			char* context = nullptr;
			uint8_t j = 0;
			#ifdef _WIN32
			char* token = strtok_s(fileBuffer, ":", &context);
			do
			{
				strcpy_s(lineBuffer[j++], token);
			}
			while (token = strtok_s(nullptr, ":", &context));
			#elif _LINUX
			char* token = strtok_r(fileBuffer, ":", &context);
			do
			{
				strncpy(lineBuffer[j++], token, sizeof(lineBuffer));
			}
			while (token = strtok_r(nullptr, ":", &context));
			#endif

			WaypointNode* node = new WaypointNode(atoi(lineBuffer[0]), Vector(atof(lineBuffer[1]), atof(lineBuffer[2]), atof(lineBuffer[3])));
			node->Flags = atoi(lineBuffer[4]);
			_WaypointNodes->push_back(node);

			// Store connected node ids for reference
			for (uint16_t k = 5; k < 260; k++)
			{
				char* content = lineBuffer[k];
				if (content[0] == '\\')
				{
					break;
				}
				else
				{
					connectedNodeIds[i][k - 5] = atoi(content);
				}
			}

			i++;
		}

		// Now handle the node connections
		for (i = 0; i < _WaypointNodes->size(); i++)
		{
			for (uint8_t j = 0; ; j++)
			{
				if (connectedNodeIds[i][j] == -1)
				{
					break;
				}
				else
				{
					for (WaypointNode* nodeToConnect : *_WaypointNodes)
					{
						if (nodeToConnect->Id == connectedNodeIds[i][j])
						{
							(*_WaypointNodes)[i]->ConnectToNode(nodeToConnect);
						}
					}
				}

				if (j == 255)
				{
					break;
				}
			}
		}

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
		for (WaypointNode* node : *_WaypointNodes)
		{
			Vector pos = node->Pos;
			file << node->Id << ":" << pos.x << ":" << pos.y << ":" << pos.z << ":" << node->Flags;

			// Also write IDs of saved nodes to file too
			for (WaypointNode* connectedNode : node->GetConnectedNodes())
			{
				file << ":" << connectedNode->Id;
			}

			file << ":\\" << std::endl;
		}

		file.close();
		Util::Log("Waypoint saved!");
	}
}

bool WaypointFileManager::_CheckDir(char* fileName) {
	struct stat info;
	stat(FILE_DIR, &info);
	if (~info.st_mode & S_IFDIR)
	{
		// Create dir
		#ifdef _WIN32
		bool created = CreateDirectory(FILE_DIR, nullptr);
		if (!created)
		#elif _LINUX
		int created = mkdir(FILE_DIR, 0755);
		if (created == -1)
		#endif
		{
			Util::Log("Error while creating directory %s!", fileName);
			return false;
		}

		Util::DebugLog("Created directory %s", fileName);
	}

	char _fileName[64];
	#ifdef _WIN32
	sprintf_s(_fileName, "%s%s.%s", FILE_DIR, _CurrentMapName, FILE_EXTENSION);
	strcpy_s(fileName, sizeof(_fileName), _fileName);
	#elif _LINUX
	sprintf(_fileName, "%s%s.%s", FILE_DIR, _CurrentMapName, FILE_EXTENSION);
	strncpy(fileName, _fileName, sizeof(_fileName));
	#endif

	return true;
}

bool WaypointFileManager::_OnLevelInit(const char* pMapName, char const* pMapEntities,
	char const* pOldLevel, char const* pLandmarkName, bool loadGame, bool background)
{
	#ifdef _WIN32
	strcpy_s(_CurrentMapName, sizeof(_CurrentMapName), pMapName);
	#elif _LINUX
	strncpy(_CurrentMapName, pMapName, sizeof(_CurrentMapName));
	#endif
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