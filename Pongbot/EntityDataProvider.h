#pragma once
#include "EntityDataType.h"
#include "EntityData.h"
#include <hlsdk/public/edict.h>

class EntityDataProvider
{
private:
	EntityDataProvider();

public:
	static void Init();
	static void Destroy();

	template<typename T>
	T GetDataFromEdict(edict_t *edict, EntityDataType dataType)
	{
		int offset = _EntityDatas[dataType].GetOffset();
		if (offset == 0)
			return 0;

		return (T) *((char*) edict->GetUnknown()->GetBaseEntity() + _EntityDatas[dataType].GetOffset());
	}

private:
	EntityData _EntityDatas[EntityDataType::ENUM_SIZE];
};

extern EntityDataProvider *_EntityDataProvider;