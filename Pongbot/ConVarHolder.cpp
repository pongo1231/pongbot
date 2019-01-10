#include "ConVarHolder.h"

ConVarHolder *_ConVarHolder;

ConVar *CVarBotAimSensivity;
ConVar *CVarBotNodeTouchRadius;
ConVar *CVarBotPosStuckPanicTime;
ConVar *CVarBotTargetPosDebugBeamTick;
ConVar *CVarBotWeaponLongRangeDist;
ConVar *CVarBotWeaponMiddleRangeDist;
ConVar *CVarBotBrainThinkTick;
ConVar *CVarBotVisibilityTick;
ConVar *CVarVisiblesProviderTick;
ConVar *CVarEntityProviderTick;
ConVar *CVarObjectiveUpdateTick;
ConVar *CVarWaypointNodeDebugBeamTick;
ConVar *CVarWaypointNodeDebugBeamDist;
ConVar *CVarEventProviderTick;
ConVar *CVarBotMaxVisibleDist;
ConVar *CVarBotEnableVisibility;
ConVar *CVarBotEnableBrain;
ConVar *CVarBotMedTargetDistance;

void ConVarHolder::Init()
{
	if (!_ConVarHolder)
	{
		_ConVarHolder = new ConVarHolder();

		_ConVarHolder->CVarBotAimSensivity = new ConVar("pongbot_bot_aimsensivity", "3.0", 0, "Bot aim sensivity");
		_ConVarHolder->CVarBotNodeTouchRadius = new ConVar("pongbot_bot_goto_nodetouchradius", "30.0", 0, "Min distance to node to count as 'touched'");
		_ConVarHolder->CVarBotPosStuckPanicTime = new ConVar("pongbot_bot_goto_stuckpanictime", "120.0", 0, "Time until bot starts panicing if stuck (crouch jump)");
		_ConVarHolder->CVarBotTargetPosDebugBeamTick = new ConVar("pongbot_bot_goto_debugbeamtick", "0.1", 0, "How often the goto debug beam gets updated");
		_ConVarHolder->CVarBotWeaponLongRangeDist = new ConVar("pongbot_bot_wep_longrangedist", "700.0", 0, "Min distance where bots use their long range weapon");
		_ConVarHolder->CVarBotWeaponMiddleRangeDist = new ConVar("pongbot_bot_wep_middlerangedist", "250.0", 0, "Min distance where bots use their middle range weapon");
		_ConVarHolder->CVarBotBrainThinkTick = new ConVar("pongbot_bot_brain_thinktick", "0.2", 0, "How often the bots will think");
		_ConVarHolder->CVarBotVisibilityTick = new ConVar("pongbot_bot_visibility_tick", "0.2", 0, "How often visibility is being tested");
		_ConVarHolder->CVarVisiblesProviderTick = new ConVar("pongbot_visibles_providetick", "1.0", 0, "How often visibles will be provided");
		_ConVarHolder->CVarEntityProviderTick = new ConVar("pongbot_entityprovide_tick", "1.0", 0, "How often all entities get iterated through");
		_ConVarHolder->CVarObjectiveUpdateTick = new ConVar("pongbot_objectiveprovider_tick", "0.5", 0, "How often all objectives get iterated through");
		_ConVarHolder->CVarWaypointNodeDebugBeamTick = new ConVar("pongbot_waypoint_debugbeamtick", "0.5", 0, "How often the node debug beams get drawn");
		_ConVarHolder->CVarWaypointNodeDebugBeamDist = new ConVar("pongbot_waypoint_debugbeamdist", "1000.0", 0, "Max draw distance of node debug beams");
		_ConVarHolder->CVarEventProviderTick = new ConVar("pongbot_eventprovider_tick", "0.2", 0, "How often the plugin checks for events");
		_ConVarHolder->CVarBotMaxVisibleDist = new ConVar("pongbot_bot_visibility_maxdist", "1500.0", 0, "How far the bots can see");
		_ConVarHolder->CVarBotEnableVisibility = new ConVar("pongbot_bot_visibility_enabled", "1", 0, "Bots should be able to see entities");
		_ConVarHolder->CVarBotEnableBrain = new ConVar("pongbot_bot_brain_enabled", "1", 0, "Bots should be able to think");
		_ConVarHolder->CVarBotMedTargetDistance = new ConVar("pongbot_bot_med_targetdistance", "50.0", 0, "Distance med bots should have to heal target");
	}
}

void ConVarHolder::Destroy()
{
	if (_ConVarHolder)
	{
		delete _ConVarHolder->CVarBotAimSensivity;
		delete _ConVarHolder->CVarBotNodeTouchRadius;
		delete _ConVarHolder->CVarBotPosStuckPanicTime;
		delete _ConVarHolder->CVarBotTargetPosDebugBeamTick;
		delete _ConVarHolder->CVarBotWeaponLongRangeDist;
		delete _ConVarHolder->CVarBotWeaponMiddleRangeDist;
		delete _ConVarHolder->CVarBotBrainThinkTick;
		delete _ConVarHolder->CVarBotVisibilityTick;
		delete _ConVarHolder->CVarVisiblesProviderTick;
		delete _ConVarHolder->CVarEntityProviderTick;
		delete _ConVarHolder->CVarObjectiveUpdateTick;
		delete _ConVarHolder->CVarWaypointNodeDebugBeamTick;
		delete _ConVarHolder->CVarWaypointNodeDebugBeamDist;
		delete _ConVarHolder->CVarBotEnableVisibility;
		delete _ConVarHolder->CVarBotEnableBrain;
		delete _ConVarHolder->CVarBotMedTargetDistance;

		delete _ConVarHolder;
	}
}