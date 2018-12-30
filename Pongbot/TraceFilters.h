#pragma once
#include "TraceHeaders.h"

class TraceFilterSimple : public CTraceFilter
{
public:
	TraceFilterSimple(IHandleEntity *passEntity, IHandleEntity *passEntity2)
		: _PassEntity(passEntity), _PassEntity2(passEntity2)
	{}

public:
	virtual bool ShouldHitEntity(IHandleEntity *iHandleEntity, int contentsMask);

private:
	const IHandleEntity *_PassEntity;
	const IHandleEntity *_PassEntity2;
	int _CollisionGroup;
};

class TraceFilterWorld : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity *iHandleEntity, int contentsMask);
};