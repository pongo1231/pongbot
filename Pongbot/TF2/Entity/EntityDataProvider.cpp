#include <stdafx.h>
#include "EntityDataProvider.h"
#include "../../Util.h"

extern IServerGameDLL* Server;

EntityDataProvider* _EntityDataProvider = nullptr;

void EntityDataProvider::Init()
{
	if (!_EntityDataProvider)
	{
		Util::DebugLog("INIT EntityDataProvider");

		_EntityDataProvider = new EntityDataProvider();
		_EntityDataProvider->UpdateOffsets();
	}
}

void EntityDataProvider::Destroy()
{
	if (_EntityDataProvider)
	{
		Util::DebugLog("DESTROY EntityDataProvider");

		delete _EntityDataProvider;
		_EntityDataProvider = nullptr;
	}
}

void EntityDataProvider::UpdateOffsets()
{
	ServerClass* serverClass = Server->GetAllServerClasses();
	while (serverClass)
	{
		const char* serverClassName = serverClass->GetName();
		SendTable* sendTable = serverClass->m_pTable;
		for (int i = 0; i < sendTable->GetNumProps(); i++)
		{
			SendProp* sendProp = sendTable->GetProp(i);
			const char* sendPropName = sendProp->GetName();
			for (int j = 0; j < _DATA_AMOUNT; j++)
			{
				EntityData& entityData = _EntityOffsets[j];
				const char* entityClass = entityData.GetEntityClass();
				const char* entityAttr = entityData.GetEntityAttr();
				if (entityData.Offset == -1 && strcmp(entityClass, serverClassName) == 0
					&& strcmp(entityAttr, sendPropName) == 0)
					{
						int offset = sendProp->GetOffset();
						entityData.Offset = offset;
						Util::DebugLog("EntityDataProvider: Offset of %s::%s is %d", entityClass, entityAttr, offset);
						break;
					}
			}
		}

		serverClass = serverClass->m_pNext;
	}

	bool offsetsNotFound = false;
	for (int i = 0; i < _DATA_AMOUNT; i++)
	{
		EntityData entityData = _EntityOffsets[i];
		if (entityData.Offset == -1)
		{
			Util::Log("!! Offset of %s::%s not found !!", entityData.GetEntityClass(),
				entityData.GetEntityAttr());
			offsetsNotFound = true;
		}
	}
	
	if (offsetsNotFound)
	{
		Util::Log("ERROR: One or more offsets were not found, this WILL cause problems");
	}
}