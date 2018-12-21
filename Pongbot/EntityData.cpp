#include "EntityData.h"
#include "Util.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/server_class.h>

extern IServerGameDLL *Server;

int _Offset;

EntityData::EntityData(const char *className, const char *key)
{
	_FindOffset(className, key);
	if (_Offset == 0)
		Util::Log("Offset of %s::%s not found!", className, key);
	else
		Util::Log("Offset of %s::%s: %d", className, key, _Offset);
}

int EntityData::GetOffset() const
{
	return _Offset;
}

void EntityData::_FindOffset(const char *className, const char *key)
{
	ServerClass *serverClass = Server->GetAllServerClasses();
	while (true)
	{
		if (strcmpi(serverClass->GetName(), className) == 0)
			break;

		serverClass = serverClass->m_pNext;
		if (!serverClass)
			return;
	}

	SendTable *sendTable = serverClass->m_pTable;
	for (int i = 0; i < sendTable->GetNumProps(); i++)
	{
		SendProp *sendProp = sendTable->GetProp(i);
		if (sendProp && strcmpi(sendProp->GetName(), key) == 0)
		{
			_Offset = sendProp->GetOffset();
			break;
		}
	}
}