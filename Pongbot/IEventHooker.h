#pragma once

class IEventHooker
{
public:
	IEventHooker();
	~IEventHooker();

	virtual void OnObjectiveUpdate()
	{}
};