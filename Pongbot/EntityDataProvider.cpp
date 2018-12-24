#include "EntityDataProvider.h"

EntityDataProvider *_EntityDataProvider;

// TODO: Maybe auto offset searching (in case it breaks)
std::map<EntityDataType, unsigned int> _EntityOffsets;

EntityDataProvider::EntityDataProvider()
{}

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