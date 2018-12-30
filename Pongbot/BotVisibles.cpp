#include "BotVisibles.h"
#include "Bot.h"
#include "BotVisiblesProvider.h"
#include "Util.h"
#include "TraceFilters.h"
#include "TFTeam.h"
#include "EntityDataProvider.h"
#include <metamod/ISmmAPI.h>

#define BOT_VISIBILITY_TICK .2f

extern IVEngineServer *Engine;
extern IEngineTrace *IIEngineTrace;
extern BotVisiblesProvider *_BotVisiblesProvider;
extern IServerGameClients *IIServerGameClients;
extern IPlayerInfoManager *IIPlayerInfoManager;

static bool _DrawDebugBeams = false;

Bot *_MBot;
std::vector<BotVisibleTarget*> _VisibleTargets;
float _TickTime;

BotVisibles::BotVisibles(Bot *bot) : _MBot(bot)
{}

std::vector<BotVisibleTarget*> BotVisibles::GetVisibleTargets() const
{
	return _VisibleTargets;
}

BotVisibleTarget *BotVisibles::GetMostImportantTarget() const
{
	Vector botPos = _MBot->GetPos();
	BotVisibleTarget *importantTarget = nullptr;
	float importantTargetDist = 9999.f;
	for (BotVisibleTarget *visibleTarget : _VisibleTargets)
	{
		BotTargetPriority targetPriority = visibleTarget->Priority;
		float targetDist = visibleTarget->Pos.DistTo(botPos);
		if (targetPriority != FRIENDLY && (!importantTarget || targetPriority > importantTarget->Priority
			|| targetDist < importantTargetDist))
		{
			importantTarget = visibleTarget;
			importantTargetDist = targetDist;
		}
	}

	return importantTarget;
}

void BotVisibles::OnThink()
{
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
	for (edict_t *edict : _BotVisiblesProvider->GetVisibleEdicts())
	{
		if (edict == botEdict)
			continue;

		Vector edictPos = Util::GetEdictOrigin(edict);
		// Target center instead of feet if edict is player
		Vector earPos = Vector();
		IIServerGameClients->ClientEarPosition(edict, &earPos);
		if (!earPos.IsZero())
			edictPos += Vector(0.f, 0.f, (earPos.z - edictPos.z) / 2.f);

		Ray_t traceLine;
		traceLine.Init(botPos, edictPos);
		trace_t traceResult;
		IIEngineTrace->TraceRay(traceLine, MASK_SOLID_BRUSHONLY, &TraceFilterSimple(botPassEntity, 
			edict->GetIServerEntity(), COLLISION_GROUP_NONE), &traceResult);
		bool traceHit = traceResult.DidHit();

		if (!traceHit)
		{
			// Insert according to distance bot <-> edict
			uint8_t insertIndex = 0;
			vec_t edictBotDistance = edictPos.DistTo(botPos);
			for (uint8_t i = 0; i < _VisibleTargets.size(); i++)
			{
				if (_VisibleTargets[i]->Pos.DistTo(botPos) >= edictBotDistance)
				{
					insertIndex = i;
					break;
				}
			}

			_AddEntity(edict, edictPos, insertIndex);
		}

		if (_DrawDebugBeams)
				Util::DrawBeam(botPos, edictPos, traceHit ? 255 : 0, traceHit ? 0 : 255, 0, BOT_VISIBILITY_TICK);
	}
}

void BotVisibles::_AddEntity(edict_t *edict, Vector edictPos, uint8_t insertIndex)
{
	BotTargetPriority targetPriority = BotTargetPriority::FRIENDLY;

	IPlayerInfo *playerInfo = IIPlayerInfoManager->GetPlayerInfo(edict);
	if (playerInfo && playerInfo->IsPlayer() && playerInfo->GetTeamIndex() != _MBot->GetTeam())
		targetPriority = BotTargetPriority::NORMAL;
		
	_VisibleTargets.insert(_VisibleTargets.begin() + insertIndex,
		new BotVisibleTarget(edictPos, targetPriority));
}

CON_COMMAND(pongbot_bot_visibility_debug, "Toggle debug visibility raytracing beams")
{
	_DrawDebugBeams = !_DrawDebugBeams;
	if (_DrawDebugBeams)
		Util::Log("Enabled debug visibility raytracing beams");
	else
		Util::Log("Disabled debug visibility raytracing beams");
}