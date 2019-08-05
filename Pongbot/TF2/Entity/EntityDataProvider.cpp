#include <stdafx.h>
#include "EntityDataProvider.h"
#include "../../Util.h"

EntityDataProvider* _EntityDataProvider = nullptr;

void EntityDataProvider::Init()
{
	if (!_EntityDataProvider)
	{
		Util::DebugLog("INIT EntityDataProvider");

		_EntityDataProvider = new EntityDataProvider();
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