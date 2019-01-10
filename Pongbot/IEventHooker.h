#pragma once

class IEventHooker
{
public:
	IEventHooker();
	~IEventHooker();

public:
	virtual void OnObjectiveUpdate()
	{}
};