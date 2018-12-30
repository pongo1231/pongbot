#include "TraceFilters.h"

const IHandleEntity *_PassEntity;
const IHandleEntity *_PassEntity2;
int m_collisionGroup;

bool TraceFilterSimple::ShouldHitEntity(IHandleEntity *iHandleEntity, int contentsMask)
{
	return iHandleEntity != _PassEntity && iHandleEntity != _PassEntity2 && contentsMask & CONTENTS_BLOCKLOS;
}

bool TraceFilterWorld::ShouldHitEntity(IHandleEntity *iHandleEntity, int contentsMask)
{
	return contentsMask & CONTENTS_BLOCKLOS;
}