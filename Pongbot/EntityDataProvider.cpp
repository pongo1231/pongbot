#include "EntityDataProvider.h"

EntityDataProvider *_EntityDataProvider;

void EntityDataProvider::Init()
{
	if (!_EntityDataProvider)
		_EntityDataProvider = new EntityDataProvider();
}

void EntityDataProvider::Destroy()
{
	if (_EntityDataProvider)
		delete _EntityDataProvider;
}