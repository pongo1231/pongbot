#include "IConVarBase.h"
#include "Util.h"
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