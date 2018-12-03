#include "GameFramable.h"
#include "Main.h"

extern Main _Main;

GameFramable::GameFramable() {
	_Main.RegisterGameFramable(this);
}

GameFramable::~GameFramable() {
	_Main.UnregisterGameFramable(this);
}