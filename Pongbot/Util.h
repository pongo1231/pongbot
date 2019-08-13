#pragma once
#include <hlsdk/public/mathlib/mathlib.h>
#include <cstdint>
#include <vector>

class Player;
struct edict_t;
class Bot;
class CGameTrace;
class ITraceFilter;

typedef CGameTrace trace_t;

namespace Util
{
	int RandomInt(int min, int max);
	float RandomFloat(float min, float max);
	void Log(const char* text, ...);
	void DebugLog(const char* text, ...);
	float DistanceToNoZ(Vector a, Vector b);
	void DrawBeam(Vector startPos, Vector endPos, uint8_t r, uint8_t g, uint8_t b, float lifeTime);
	std::vector<Player> GetAllPlayers();
	void TraceLine(Vector startPos, Vector targetPos, unsigned int fMask, ITraceFilter* traceFilter,
		trace_t* traceResult);

	QAngle GetLookAtAngleForPos(Bot* bot, Vector lookAtPos);
	Vector2D GetIdealMoveSpeedsToPos(Bot* bot, Vector targetPos);
	QAngle CorrectViewAngle(QAngle viewAngle);
	static vec_t _NormalizeAngle(vec_t angle);
	static vec_t _ClampAngle(vec_t angle, float min, float max);
}