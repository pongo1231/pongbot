#include "BotVisibles.h"
#include "Bot.h"
#include "BotVisiblesProvider.h"
#include "Util.h"
#include "TraceFilters.h"
#include "TFTeam.h"
#include <metamod/ISmmAPI.h>

#define BOT_VISIBILITY_TICK 0.2

extern IVEngineServer *Engine;
extern IEngineTrace *IIEngineTrace;
extern BotVisiblesProvider *_BotVisiblesProvider;

static bool _DrawDebugBeams = false;

Bot *_MBot;
std::vector<edict_t*> _VisibleEdicts;
float _TickTime;

BotVisibles::BotVisibles(Bot *bot) : _MBot(bot) {}

std::vector<edict_t*> BotVisibles::GetVisibleEdicts() const {
	return _VisibleEdicts;
}

void BotVisibles::OnThink() {
	if (_MBot->GetTeam() == TFTeam::SPECTATOR || _MBot->IsDead())
		return;

	float currentTime = Engine->Time();

	if (_TickTime > currentTime)
		return;

	_TickTime = currentTime + BOT_VISIBILITY_TICK;

	_VisibleEdicts.clear();

	Vector botPos = _MBot->GetEarPos();
	edict_t *botEdict = _MBot->GetEdict();
	IHandleEntity *botPassEntity = botEdict->GetIServerEntity();

	for (edict_t *edict : _BotVisiblesProvider->GetAllEdicts()) {
		if (edict == botEdict)
			continue;

		Vector edictPos = Util::GetEdictOrigin(edict);

		Ray_t traceLine;
		traceLine.Init(botPos, edictPos);

		trace_t traceResult;

		IIEngineTrace->TraceRay(traceLine, MASK_SOLID, &TraceFilterSimple(botPassEntity, 
			edict->GetIServerEntity(), COLLISION_GROUP_NONE), &traceResult);

		bool traceHit = traceResult.DidHit();

		if (_DrawDebugBeams)
			Util::DrawBeam(botPos, edictPos, traceHit ? 255 : 0, traceHit ? 0 : 255, 0, BOT_VISIBILITY_TICK);

		if (!traceHit) {
			// Insert according to distance bot <-> edict
			vec_t edictBotDistance = edictPos.DistTo(botPos);

			if (_VisibleEdicts.size() == 0)
				_VisibleEdicts.push_back(edict);
			else
				for (uint8_t i = 0; i < _VisibleEdicts.size(); i++) {
					if (Util::GetEdictOrigin(_VisibleEdicts[i]).DistTo(botPos) >= edictBotDistance) {
						_VisibleEdicts.insert(_VisibleEdicts.begin() + i, edict);

						break;
					}
				}
		}
	}
}

CON_COMMAND(pongbot_bot_visibility_debug, "Toggle debug visibility raytracing beams") {
	_DrawDebugBeams = !_DrawDebugBeams;

	if (_DrawDebugBeams)
		Util::Log("Enabled debug visibility raytracing beams");
	else
		Util::Log("Disabled debug visibility raytracing beams");
}