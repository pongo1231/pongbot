#include "Util.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/mathlib/vector.h>
#include <hlsdk/game/shared/IEffects.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <cstdlib>

extern IVEngineServer *Engine;
extern IPlayerInfoManager *IIPlayerInfoManager;
extern ISmmAPI *g_SMAPI;
extern IEffects *IIEffects;

namespace Util {
	int RandomInt(int min, int max) {
		if (min >= max)
			return min;
		return rand() % (max - min + 1) + min;
	}

	void Log(const char *text, ...) {
		char userText[64], prefixedText[128];
		va_list args;
		va_start(args, text);
		vsnprintf_s(userText, sizeof(userText), text, args);
		va_end(args);
		sprintf_s(prefixedText, "[Pongbot] %s\n", userText);
		g_SMAPI->ConPrintf(prefixedText);
	}

	float DistanceToNoZ(Vector a, Vector b) {
		return abs((a.x + a.y) - (b.x + b.y));
	}

	void DrawBeam(Vector startPos, Vector endPos, uint8_t r, uint8_t g, uint8_t b) {
		IIEffects->Beam(startPos, endPos, Engine->PrecacheModel("sprites/lgtning.vmt"),
			0, 0, 1, 1, 1, 1, 255, 1, r, g, b, 255, 10);
	}

	std::vector<IPlayerInfo*> GetAllPlayers() {
		std::vector<IPlayerInfo*> players;
		for (uint8_t i = 1; i < 33; i++) {
			edict_t *edict = Engine->PEntityOfEntIndex(i);
			if (edict) {
				IPlayerInfo *playerInfo = IIPlayerInfoManager->GetPlayerInfo(edict);
				if (playerInfo && playerInfo->IsPlayer())
					players.push_back(playerInfo);
			}
		}
		return players;
	}
}