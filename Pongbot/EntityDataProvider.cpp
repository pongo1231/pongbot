#include "EntityDataProvider.h"

EntityDataProvider *_EntityDataProvider;

void EntityDataProvider::Init()
{
	Assert(!_EntityDataProvider);
	_EntityDataProvider = new EntityDataProvider();
}

void EntityDataProvider::Destroy()
{
	Assert(_EntityDataProvider);
	delete _EntityDataProvider;
}