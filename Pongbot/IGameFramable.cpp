#include "IGameFramable.h"
#include "Main.h"

extern Main _Main;

IGameFramable::IGameFramable()
{
	_Main.RegisterIGameFramable(this);
}

IGameFramable::~IGameFramable()
{
	_Main.UnregisterIGameFramable(this);
}