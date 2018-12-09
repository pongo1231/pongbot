#include "BotVisibles.h"
#include "Bot.h"
#include "BotVisiblesProvider.h"
#include "Util.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/raytrace.h>

extern IVEngineServer *Engine;
extern BotVisiblesProvider *_BotVisiblesProvider;

Bot *_MBot;
std::vector<edict_t*> _VisibleEdicts;

BotVisibles::BotVisibles(Bot *bot) : _MBot(bot) {}

std::vector<edict_t*> BotVisibles::GetVisibleEdicts() const {
	return _VisibleEdicts;
}

void BotVisibles::OnGameFrame() {
	static float waitTime;
	float currentTime = Engine->Time();
	if (waitTime > currentTime)
		return;
	waitTime = currentTime + 0.2;

	_VisibleEdicts.clear();

	Vector botRayTracePos = _MBot->GetEarPos();
	botRayTracePos.z += 100;

	for (edict_t *edict : _BotVisiblesProvider->GetAllEdicts()) {
		// TODO: raytracing and shit
		if (edict != _MBot->GetEdict()) {
			Vector edictPos = Util::GetEdictOrigin(edict);

			RayStream rayStream = RayStream();
			RayTracingSingleResult rayTraceResult;
			RayTracingEnvironment().AddToRayStream(rayStream, botRayTracePos, edictPos, &rayTraceResult);

			Util::DrawBeam(botRayTracePos, edictPos, 255, 255, 255, 0.2);
			Util::Log("%d %f %f %f", rayTraceResult.HitID, rayTraceResult.ray_length, rayTraceResult.HitDistance, edictPos.DistToSqr(botRayTracePos));
			if (rayTraceResult.HitDistance == edictPos.DistToSqr(botRayTracePos))
				_VisibleEdicts.push_back(edict);
		}
	}
}