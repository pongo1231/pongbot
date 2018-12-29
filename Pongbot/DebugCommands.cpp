#include "IConVarBase.h"
#include "Util.h"
#include "EntityProvider.h"
#include "EntityDataProvider.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/edict.h>
#include <hlsdk/public/server_class.h>
#include <fstream>

extern IServerGameDLL *Server;

CON_COMMAND(pongbot_debug_dumpoffsets, "Dumps all offsets to addons/pongbot/offsets_dump.txt")
{
	std::ofstream file("tf/addons/pongbot/offsets_dump.txt");

	ServerClass *serverClass = Server->GetAllServerClasses();
	while (serverClass)
	{
		SendTable *sendTable = serverClass->m_pTable;
		for (int i = 0; i < sendTable->GetNumProps(); i++)
		{
			SendProp *sendProp = sendTable->GetProp(i);
			file << serverClass->GetName() << "::" << sendProp->GetName() << " = " << sendProp->GetOffset() << std::endl;
		}

		serverClass = serverClass->m_pNext;
	}

	Util::Log("Took a dump successfully!");
	file.close();
}

CON_COMMAND(pongbot_debug_getdata, "Outputs specified entity data")
{
	const char *data = args.Arg(1);
	if (strcmp(data, "") == 0)
		Util::Log("FLAG_OWNER | FLAG_STATUS");
	else
		if (data[0] == 'F' && data[1] == 'L' && data[2] == 'A' && data[3] == 'G')
		{
			std::vector<edict_t*> itemFlags = _EntityProvider->SearchEdictsByClassname("item_teamflag");
			for (int i = 0; i < itemFlags.size(); i++)
				Util::Log("Flag %d: %d", i, _EntityDataProvider->GetDataFromEdict<int>(itemFlags[i],
					strcmp(data, "FLAG_OWNER") == 0 ? FLAG_OWNER : FLAG_STATUS));
		}
}