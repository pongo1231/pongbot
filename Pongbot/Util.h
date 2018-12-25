#pragma once
#include <hlsdk/public/mathlib/mathlib.h>
#include <cstdint>
#include <vector>

class IPlayerInfo;
struct edict_t;
class Bot;

namespace Util
{
	int RandomInt(int min, int max);
	void Log(const char *text, ...);
	float DistanceToNoZ(Vector a, Vector b);
	void DrawBeam(Vector startPos, Vector endPos, uint8_t r, uint8_t g, uint8_t b, float lifeTime);
	std::vector<edict_t*> GetAllPlayers();
	Vector GetEdictOrigin(edict_t *edict);

	QAngle GetLookAtAngleForPos(Bot *bot, Vector lookAtPos);
	Vector2D GetIdealMoveSpeedsToPos(Bot *bot, Vector targetPos);
	QAngle CorrectViewAngle(QAngle viewAngle);
	static vec_t _NormalizeAngle(vec_t angle);
	static vec_t _ClampAngle(vec_t angle, float min, float max);
}

