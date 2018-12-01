#include "TF2Helper.h"

namespace TF2Helper {
	int GetClassSpeed(TFClass tfClass) {
		switch (tfClass) {
			case SCOUT: return 133;
			case SOLDIER: return 80;
			case PYRO: return 100;
			case DEMO: return 93;
			case HEAVY: return 77;
			case ENGI: return 100;
			case MED: return 109;
			case SNIPER: return 100;
			case SPY: return 109;
		}
		return -1;
	}
}