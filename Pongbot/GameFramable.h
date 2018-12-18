#pragma once

class GameFramable
{
public:
	GameFramable();
	~GameFramable();

	virtual void OnGameFrame() = 0;
};