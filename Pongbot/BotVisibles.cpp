#include "BotVisibles.h"
#include "Bot.h"
#include "BotVisiblesProvider.h"
#include "Util.h"
#include "TraceFilters.h"
#include "TFTeam.h"
#include "EntityDataProvider.h"
#include <metamod/ISmmAPI.h>
#include <hlsdk/public/mathlib/mathlib.h>
#include <numeric>

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
	float importantTargetDist = 99999.f;
	for (BotVisibleTarget *visibleTarget : _VisibleTargets)
	{
		BotTargetPriority targetPriority = visibleTarget->Priority;
		float targetDist = visibleTarget->Pos.DistTo(botPos);
		if (targetPriority != PRIORITY_FRIENDLY && (!importantTarget || targetPriority > importantTarget->Priority
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
	for (edict_t *edict : _BotVisiblesProvider->GetVisibleEdicts())
	{
		if (edict == _MBot->GetEdict())
			continue;

		Vector edictPos = Util::GetEdictOrigin(edict);
		if (_IsTargetInSight(edictPos))
		{
			// Target center instead of feet if entity is player
			Vector earPos;
			IIServerGameClients->ClientEarPosition(edict, &earPos);
			if (!earPos.IsZero())
				edictPos += Vector(0.f, 0.f, (earPos.z - edictPos.z) / 2.f);

			bool clearLine = _HasClearLineToTarget(edict->GetIServerEntity(), edictPos);
			if (clearLine)
			{
				// Insert according to distance bot <-> edict
				uint8_t insertIndex = 0;
				vec_t edictBotDistance = edictPos.DistTo(botPos);
				for (uint8_t i = 0; i < _VisibleTargets.size(); i++)
					if (_VisibleTargets[i]->Pos.DistTo(botPos) >= edictBotDistance)
					{
						insertIndex = i;
						break;
					}

				_AddEntity(edict, edictPos, insertIndex);
			}

			if (_DrawDebugBeams)
				Util::DrawBeam(botPos, edictPos, clearLine ? 255 : 0, clearLine ? 0 : 255, 0, BOT_VISIBILITY_TICK);
		}
	}
}

void BotVisibles::_AddEntity(edict_t *edict, Vector edictPos, uint8_t insertIndex)
{
	BotTargetPriority targetPriority = PRIORITY_FRIENDLY;

	IPlayerInfo *playerInfo = IIPlayerInfoManager->GetPlayerInfo(edict);
	if (playerInfo && playerInfo->IsPlayer() && playerInfo->GetTeamIndex() != _MBot->GetTeam())
		targetPriority = PRIORITY_NORMAL;
		
	_VisibleTargets.insert(_VisibleTargets.begin() + insertIndex,
		new BotVisibleTarget(edictPos, targetPriority));
}

bool BotVisibles::_IsTargetInSight(Vector targetPos) const
{
	Vector vectorForward;
	AngleVectors(_MBot->GetViewAngle(), &vectorForward);
	float dot = vectorForward.x * targetPos.x + vectorForward.y * targetPos.y + vectorForward.z * targetPos.z;
	float angle = std::acos(dot / (vectorForward.Length() * targetPos.Length()));

	// TODOOOOOOOOOOOOOOO!
	return true;
}

bool BotVisibles::_HasClearLineToTarget(IServerEntity *targetEntity, Vector targetPos) const
{
	Ray_t traceLine;
	traceLine.Init(_MBot->GetEarPos(), targetPos);
	trace_t traceResult;
	IIEngineTrace->TraceRay(traceLine, MASK_SOLID, &TraceFilterSimple(_MBot->GetEdict()->GetIServerEntity(),
		targetEntity), &traceResult);

	return !traceResult.DidHit();
}

CON_COMMAND(pongbot_bot_visibility_debug, "Toggle debug visibility raytracing beams")
{
	_DrawDebugBeams = !_DrawDebugBeams;
	if (_DrawDebugBeams)
		Util::Log("Enabled debug visibility raytracing beams");
	else
		Util::Log("Disabled debug visibility raytracing beams");
}