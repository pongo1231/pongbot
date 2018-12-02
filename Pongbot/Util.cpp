#include "Util.h"
#include <metamod/ISmmAPI.h>
#include <cstdlib>

extern ISmmAPI *g_SMAPI;

namespace Util {
	int RandomInt(int min, int max) {
		if (min >= max)
			return min;
		return rand() % (max - min + 1) + min;
	}

	void Log(const char *text, ...) {
		char fullText[64], prefixedText[128];
		va_list args;
		va_start(args, text);
		vsnprintf_s(fullText, sizeof(fullText), text, args);
		va_end(args);
		sprintf_s(prefixedText, "[Pongbot] %s\n", fullText);
		g_SMAPI->ConPrintf(prefixedText);
	}
}