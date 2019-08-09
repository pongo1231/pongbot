#include <stdafx.h>
#include "../IConVarBase.h"
#include "../Util.h"
#include "Entity/EntityProvider.h"
#include "Entity/CTFFlag.h"
#include "Entity/Player.h"
#include "Entity/EntityDataProvider.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/edict.h>
#include <hlsdk/public/server_class.h>
#include <fstream>

extern IVEngineServer *Engine;
extern IServerGameDLL *Server;

CON_COMMAND(pongbot_debug_dumpoffsets, "Dumps all offsets to addons/pongbot/offsets_dump.txt")
{
	std::ofstream file("tf/addons/pongbot/offsets_dump.txt");

	ServerClass* serverClass = Server->GetAllServerClasses();
	while (serverClass)
	{
		SendTable* sendTable = serverClass->m_pTable;
		for (int i = 0; i < sendTable->GetNumProps(); i++)
		{
			SendProp* sendProp = sendTable->GetProp(i);
			file << serverClass->GetName() << "::" << sendProp->GetName() << " = " << sendProp->GetOffset() << std::endl;
		}

		serverClass = serverClass->m_pNext;
	}

	Util::Log("Took a dump successfully!");
	file.close();
}

CON_COMMAND(pongbot_debug_getdata, "Outputs specified entity data")
{
	const char* data = args.Arg(1);
	if (strcmp(data, "") == 0)
	{
		Util::Log("FLAG_OWNER | FLAG_STATUS | PLAYER_CURRENTWEAPON | PLAYER_HEALTH");
	}
	else
	{
		if (strcmp(data, "FLAG_OWNER") == 0)
		{
			std::vector<Entity> itemFlags = _EntityProvider->SearchEntitiesByClassname("item_teamflag");
			for (unsigned int i = 0; i < itemFlags.size(); i++)
			{
				Util::Log("Flag %d: %d", i, CTFFlag(itemFlags[i]).GetOwner());
			}
		}
		else if (strcmp(data, "FLAG_STATUS") == 0)
		{
			std::vector<Entity> itemFlags = _EntityProvider->SearchEntitiesByClassname("item_teamflag");
			for (unsigned int i = 0; i < itemFlags.size(); i++)
			{
				Util::Log("Flag %d: %d", i, CTFFlag(itemFlags[i]).GetStatus());
			}
		}
		else if (strcmp(data, "PLAYER_CURRENTWEAPON") == 0)
		{
			std::vector<Player> players = Util::GetAllPlayers();
			for (unsigned int i = 0; i < players.size(); i++)
			{
				Util::Log("Player %d: %d", i, _EntityDataProvider->GetDataFromEntity<int>(players[i], DATA_PLAYER_CURRENTWEAPON));
			}
		}
		else if (strcmp(data, "PLAYER_HEALTH") == 0)
		{
			std::vector<Player> players = Util::GetAllPlayers();
			for (unsigned int i = 0; i < players.size(); i++)
			{
				Util::Log("Player %d: %f", i, players[i].GetHealth());
			}
		}
	}
}