#include "stdafx.h"
#include "ConVarHolder.h"
#include "Util.h"

ConVarHolder* _ConVarHolder = nullptr;

void ConVarHolder::Init()
{
	if (!_ConVarHolder)
	{
		Util::DebugLog("INIT ConVarHolder");
		_ConVarHolder = new ConVarHolder();

		_ConVarHolder->CVarBotAimSensivity = new ConVar("pongbot_bot_aimsensivity", "3.0", 0, "Bot aim sensivity");
		_ConVarHolder->CVarBotPosStuckPanicTime = new ConVar("pongbot_bot_goto_stuckpanictime", "120.0", 0, "Time until bot starts panicking if stuck (crouch jump)");
		_ConVarHolder->CVarBotPosStuckPanicTries = new ConVar("pongbot_bot_goto_stuckpanictries", "3", 0, "Panic Crouch Jump tries until bots try to take special action");
		_ConVarHolder->CVarBotPosStuckPanicRange = new ConVar("pongbot_bot_goto_stuckpanicrange", "10.0", 0, "Max dist from last pos bot has to be at to count as stuck");		
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
		_ConVarHolder->CVarWaypointNodeDefaultRange = new ConVar("pongbot_waypoint_nodedefaultrange", "30.0", 0, "Default range of newly created waypoint nodes");		
		_ConVarHolder->CVarEventProviderTick = new ConVar("pongbot_eventprovider_tick", "0.2", 0, "How often the plugin checks for events");
		_ConVarHolder->CVarBotMaxVisibleDist = new ConVar("pongbot_bot_visibility_maxdist", "1500.0", 0, "How far the bots can see");
		_ConVarHolder->CVarBotEnableVisibility = new ConVar("pongbot_bot_visibility_enabled", "1", 0, "Bots should be able to see entities");
		_ConVarHolder->CVarBotEnableBrain = new ConVar("pongbot_bot_brain_enabled", "1", 0, "Bots should be able to think");
		_ConVarHolder->CVarBotMedTargetDistance = new ConVar("pongbot_bot_med_targetdistance", "100.0", 0, "Distance med bots should have to heal target");
		_ConVarHolder->CVarBotMovementIgnoreRadius = new ConVar("pongbot_bot_movement_ignoreradius", "5.0", 0, "Radius around bot where movement tasks should be ignored");
		_ConVarHolder->CVarBotSniperCampTime = new ConVar("pongbot_bot_sniper_camp_time", "60", 0, "Time in seconds after which snipers might consider moving to another camping spot (if one exists)");		
	}
}

void ConVarHolder::Destroy()
{
	if (_ConVarHolder)
	{
		delete _ConVarHolder->CVarBotAimSensivity;
		delete _ConVarHolder->CVarBotPosStuckPanicTime;
		delete _ConVarHolder->CVarBotPosStuckPanicTries;
		delete _ConVarHolder->CVarBotPosStuckPanicRange;
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
		delete _ConVarHolder->CVarWaypointNodeDefaultRange;
		delete _ConVarHolder->CVarBotEnableVisibility;
		delete _ConVarHolder->CVarBotEnableBrain;
		delete _ConVarHolder->CVarBotMedTargetDistance;
		delete _ConVarHolder->CVarBotMovementIgnoreRadius;
		delete _ConVarHolder->CVarBotSniperCampTime;

		delete _ConVarHolder;
		_ConVarHolder = nullptr;
	}
}