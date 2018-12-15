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
extern IServerGameClients *IIServerGameClients;

static bool _DrawDebugBeams = false;

Bot *_MBot;
std::vector<BotVisibleTarget*> _VisibleTargets;
float _TickTime;

BotVisibles::BotVisibles(Bot *bot) : _MBot(bot) {}

std::vector<BotVisibleTarget*> BotVisibles::GetVisibleTargets() const {
	return _VisibleTargets;
}

void BotVisibles::OnThink() {
	if (_MBot->IsDead())
		return;

	float currentTime = Engine->Time();
	if (_TickTime > currentTime)
		return;
	_TickTime = currentTime + BOT_VISIBILITY_TICK;

	for (BotVisibleTarget *visibleTarget : _VisibleTargets)
		delete visibleTarget;
	_VisibleTargets.clear();

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

		if (!traceHit) {
			// Insert according to distance bot <-> edict
			uint8_t insertIndex = 0;
			vec_t edictBotDistance = edictPos.DistTo(botPos);
			for (uint8_t i = 0; i < _VisibleTargets.size(); i++) {
				if (_VisibleTargets[i]->Pos.DistTo(botPos) >= edictBotDistance) {
					insertIndex = i;
					break;
				}
			}

			// Target center instead of feet if edict is player
			Vector earPos = Vector();
			IIServerGameClients->ClientEarPosition(edict, &earPos);
			if (!earPos.IsZero())
				edictPos.z = (edictPos.z + earPos.z) / 2;

			// TODO: Target Priorities
			_VisibleTargets.insert(_VisibleTargets.begin() + insertIndex,
				new BotVisibleTarget(edictPos, BotTargetPriority::NORMAL));
		}

		if (_DrawDebugBeams)
				Util::DrawBeam(botPos, edictPos, traceHit ? 255 : 0, traceHit ? 0 : 255, 0, BOT_VISIBILITY_TICK);
	}
}

CON_COMMAND(pongbot_bot_visibility_debug, "Toggle debug visibility raytracing beams") {
	_DrawDebugBeams = !_DrawDebugBeams;
	if (_DrawDebugBeams)
		Util::Log("Enabled debug visibility raytracing beams");
	else
		Util::Log("Disabled debug visibility raytracing beams");
}