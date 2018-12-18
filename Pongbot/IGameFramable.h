#pragma once

class IGameFramable
{
public:
	IGameFramable();
	~IGameFramable();

	virtual void OnGameFrame() = 0;
};