#include "TraceFilters.h"

const IHandleEntity *_PassEntity;
const IHandleEntity *_PassEntity2;
int m_collisionGroup;

TraceFilterSimple::TraceFilterSimple(IHandleEntity *passEntity, IHandleEntity *passEntity2,
	int collisionGroup) : _PassEntity(passEntity), _PassEntity2(passEntity2),
	_CollisionGroup(collisionGroup)
{}

bool TraceFilterSimple::ShouldHitEntity(IHandleEntity *iHandleEntity, int contentsMask)
{
	return iHandleEntity != _PassEntity && iHandleEntity != _PassEntity2;
}