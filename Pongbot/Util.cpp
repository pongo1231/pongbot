#include "stdafx.h"
#include "Util.h"
#include "Bot/Bot.h"
#include "TF2/Class/TFClassInfoProvider.h"
#include "TF2/Entity/Player.h"
#include "TF2/Trace/TraceFilters.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/mathlib/mathlib.h>
#include <hlsdk/game/shared/IEffects.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <cstdlib>
#include <algorithm>
#include <cmath>

extern IVEngineServer* Engine;
extern IPlayerInfoManager* IIPlayerInfoManager;
extern ISmmAPI* g_SMAPI;
extern IEffects* IIEffects;
extern IEngineTrace* IIEngineTrace;

namespace Util
{
	int RandomInt(int min, int max)
	{
		if (min >= max)
		{
			return min;
		}

		return rand() % (max - min + 1) + min;
	}

	float RandomFloat(float min, float max)
	{
		if (min >= max)
		{
			return min;
		}

		return std::fmod(rand(), (max - min + 1.f) + min);
	}

	void Log(const char* text, ...)
	{
		char userText[512], prefixedText[524];
		va_list args;
		va_start(args, text);
		vsnprintf(userText, sizeof(userText), text, args);
		va_end(args);
		sprintf(prefixedText, "[Pongbot] %s\n", userText);

		g_SMAPI->ConPrintf(prefixedText);
	}

	void DebugLog(const char* text, ...)
	{
		#ifdef _DEBUG
		char userText[512], prefixedText[524];
		va_list args;
		va_start(args, text);
		vsnprintf(userText, sizeof(userText), text, args);
		va_end(args);
		sprintf(prefixedText, "[Pongbot | DEBUG] %s\n", userText);

		g_SMAPI->ConPrintf(prefixedText);
		#endif
	}

	float DistanceToNoZ(Vector a, Vector b)
	{
		return abs((a.x + a.y) - (b.x + b.y));
	}

	void DrawBeam(Vector startPos, Vector endPos, uint8_t r, uint8_t g, uint8_t b, float lifeTime)
	{
		IIEffects->Beam(startPos, endPos, Engine->PrecacheModel("sprites/lgtning.vmt"), 0, 0, 1,
			lifeTime, 1, 1, 255, 1, r, g, b, 255, 10);
	}

	std::vector<Player> GetAllPlayers()
	{
		std::vector<Player> players;
		for (uint8_t i = 1; i < 33; i++)
		{
			Player player(Engine->PEntityOfEntIndex(i));
			if (player.Exists())
			{
				players.push_back(player);
			}
		}

		return players;
	}

	void TraceLine(Vector startPos, Vector targetPos, unsigned int fMask, ITraceFilter* traceFilter,
		trace_t* traceResult)
	{
		Ray_t traceLine;
		traceLine.Init(startPos, targetPos);
		IIEngineTrace->TraceRay(traceLine, fMask, traceFilter, traceResult);
	}

	Player GetPlayerFromUserId(int userId)
	{
		if (userId < 0)
		{
			return Player();
		}

		for (Player player : GetAllPlayers())
		{
			if (player.GetUserId() == userId)
			{
				return player;
			}
		}

		return Player();
	}

	QAngle GetLookAtAngleForPos(Bot* bot, Vector lookAtPos)
	{
		Vector vectorAngle = lookAtPos - bot->GetEarPos();
		QAngle angle;
		VectorAngles(vectorAngle / vectorAngle.Length(), angle);

		return CorrectViewAngle(angle);
	}

	Vector2D GetIdealMoveSpeedsToPos(Bot* bot, Vector targetPos)
	{
		Vector2D sins;
		SinCos(DEG2RAD(bot->GetViewAngle().y - GetLookAtAngleForPos(bot, targetPos).y), &sins.y, &sins.x);
		sins = sins / sins.Length() * _TFClassInfoProvider->GetClassInfo(bot->GetClass()).GetSpeed() * 3.f;

		return sins;
	}

	QAngle CorrectViewAngle(QAngle viewAngle)
	{
		return QAngle(_ClampAngle(_NormalizeAngle(viewAngle.x), -89.f, 89.f), _NormalizeAngle(viewAngle.y), 0.f);
	}

	static vec_t _NormalizeAngle(vec_t angle)
	{
		return std::remainderf(angle, 360.f);
	}

	static vec_t _ClampAngle(vec_t angle, float min, float max)
	{
		return std::max(min, std::min(angle, max));
	}
}