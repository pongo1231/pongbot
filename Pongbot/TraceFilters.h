#pragma once
#include "TraceHeaders.h"

class TraceFilterSimple : public CTraceFilter
{
public:
	TraceFilterSimple(IHandleEntity *passEntity, IHandleEntity *passEntity2, int collisionGroup);
	bool ShouldHitEntity(IHandleEntity *iHandleEntity, int contentsMask);

private:
	const IHandleEntity *_PassEntity;
	const IHandleEntity *_PassEntity2;
	int _CollisionGroup;
};