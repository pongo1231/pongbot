#include "stdafx.h"
#include "IEventHooker.h"
#include "EventHooksProvider.h"

IEventHooker::IEventHooker()
{
	_EventHooksProvider->AddEventHooker(this);
}

IEventHooker::~IEventHooker()
{
	_EventHooksProvider->RemoveEventHooker(this);
}