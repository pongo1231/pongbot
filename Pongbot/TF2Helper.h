#pragma once

enum TFClass {
	SCOUT,
	SOLDIER,
	PYRO,
	DEMO,
	HEAVY,
	ENGI,
	MED,
	SNIPER,
	SPY
};

namespace TF2Helper {
	int GetClassSpeed(TFClass tfClass);
}