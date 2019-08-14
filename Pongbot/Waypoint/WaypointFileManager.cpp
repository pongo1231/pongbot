#include <stdafx.h>
#include "WaypointFileManager.h"
#include "../Util.h"
#include "WaypointNode.h"
#ifdef _WIN32
#include <windows.h>
#elif _LINUX
#include <sys/stat.h>
#endif
#include <fstream>
#include <stdint.h>

#define FILE_DIR "tf/addons/pongbot/waypoints/"
#define FILE_EXTENSION "pbw"

WaypointFileManager* _WaypointFileManager = nullptr;

static std::vector<WaypointNode*>* _WaypointNodes = nullptr;

void WaypointFileManager::Init(std::vector<WaypointNode*>* waypointNodes)
{
	if (!_WaypointFileManager)
	{
		Util::DebugLog("INIT WaypointFileManager");

		_WaypointNodes = waypointNodes;
		_WaypointFileManager = new WaypointFileManager();
	}
}

void WaypointFileManager::Destroy()
{
	if (_WaypointFileManager)
	{
		Util::DebugLog("DESTROY WaypointFileManager");

		delete _WaypointFileManager;
		_WaypointFileManager = nullptr;
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

		int i = 0;
		while (file.getline(fileBuffer, sizeof(fileBuffer)))
		{
			if (strlen(fileBuffer) == 0 || (fileBuffer[0] == '/' && fileBuffer[1] == '/'))
			{
				continue;
			}

			char lineBuffer[256][16];
			char* context = nullptr;
			int j = 0;
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
				strncpy(lineBuffer[j++], token, sizeof(lineBuffer[j]));
			}
			while (token = strtok_r(nullptr, ":", &context));
			#endif

			WaypointNode* node = new WaypointNode(atoi(lineBuffer[0]), Vector(atof(lineBuffer[1]), atof(lineBuffer[2]), atof(lineBuffer[3])),
				atoi(lineBuffer[4]), atof(lineBuffer[5]), QAngle(atof(lineBuffer[6]), atof(lineBuffer[7]), atof(lineBuffer[8])));
			_WaypointNodes->push_back(node);

			// Store connected node ids for reference
			int k;
			for (k = 9; k < 255 + 9; k++)
			{
				char* content = lineBuffer[k];
				if (content[0] == '\\')
				{
					break;
				}
				else
				{
					connectedNodeIds[i][k - 9] = atoi(content);
				}
			}
			connectedNodeIds[i][k - 9] = -1;

			i++;
		}

		// Now handle the node connections
		for (i = 0; i < _WaypointNodes->size(); i++)
		{
			for (int j = 0; connectedNodeIds[i][j] != -1 && j < 255; j++)
			{
				for (WaypointNode* nodeToConnect : *_WaypointNodes)
				{
					if (nodeToConnect->GetId() == connectedNodeIds[i][j])
					{
						(*_WaypointNodes)[i]->ConnectToNode(nodeToConnect);
					}
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
		file << "// Format: " << std::endl;
		file << "// ID : X : Y : Z : Flags : Range : PrefAngleX : PrefAngleY : PrefAngleZ : <Connected Node IDs> : \\" << std::endl;
		file << std::endl;

		// Write all nodes to file
		for (WaypointNode* node : *_WaypointNodes)
		{
			Vector pos = node->GetPos();
			QAngle angle = node->OptimalViewAngle;
			file << node->GetId() << ":" << pos.x << ":" << pos.y << ":" << pos.z << ":" << node->Flags << ":" << node->GetRange()
				<< ":" << angle.x << ":" << angle.y << ":" << angle.z;

			// Also write IDs of saved nodes to file too
			for (WaypointNode* connectedNode : node->GetConnectedNodes())
			{
				file << ":" << connectedNode->GetId();
			}

			file << ":\\" << std::endl;
		}

		file.close();
		Util::Log("Waypoint saved!");
	}
}

void WaypointFileManager::OnLevelInit(const char* pMapName, char const* pMapEntities, char const* pOldLevel,
		char const* pLandmarkName, bool loadGame, bool background)
{
	_OnLevelInit(pMapName);
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

bool WaypointFileManager::_OnLevelInit(const char* pMapName)
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