#include "Util.h"
#include "Bot.h"
#include "TF2Util.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/mathlib/mathlib.h>
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

	void DrawBeam(Vector startPos, Vector endPos, uint8_t r, uint8_t g, uint8_t b, float lifeTime) {
		IIEffects->Beam(startPos, endPos, Engine->PrecacheModel("sprites/lgtning.vmt"),
			0, 0, 1, lifeTime, 1, 1, 255, 1, r, g, b, 255, 10);
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

	Vector GetEdictOrigin(edict_t *edict) {
		return edict->GetCollideable()->GetCollisionOrigin();
	}

	QAngle GetLookAtAngleForPos(Bot *bot, Vector lookAtPos) {
		Vector vectorAngle = lookAtPos - bot->GetEarPos();
		QAngle angle;
		VectorAngles(vectorAngle / vectorAngle.Length(), angle);
		return angle;
	}

	Vector2D GetIdealMoveSpeedsToPos(Bot *bot, Vector targetPos) {
		Vector2D sins;
		SinCos(DEG2RAD(_GetYawAngle(bot, targetPos)), &sins.y, &sins.x);
		sins = sins / sins.Length() * TF2Util::GetClassSpeed(bot->GetClass()) * 3;
		return sins;
	}

	static vec_t _CorrectAngle(vec_t angle) {
		if (angle > 180)
			angle -= 360;
		else if (angle < -180)
			angle += 360;
		return angle;
	}

	static vec_t _GetYawAngle(Bot *bot, Vector targetPos) {
		return _CorrectAngle(bot->GetAngle().y - _CorrectAngle(GetLookAtAngleForPos(bot, targetPos).y));
	}
}