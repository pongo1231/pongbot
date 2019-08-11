#pragma once
#include <hlsdk/public/tier1/convar.h>

class ConVarHolder
{
private:
	ConVarHolder() {}

public:
	static void Init();
	static void Destroy();

	ConVar* CVarBotAimSensivity = nullptr;
	ConVar* CVarBotPosStuckPanicTime = nullptr;
	ConVar* CVarBotPosStuckPanicRange = nullptr;
	ConVar* CVarBotTargetPosDebugBeamTick = nullptr;
	ConVar* CVarBotWeaponLongRangeDist = nullptr;
	ConVar* CVarBotWeaponMiddleRangeDist = nullptr;
	ConVar* CVarBotBrainThinkTick = nullptr;
	ConVar* CVarBotVisibilityTick = nullptr;
	ConVar* CVarVisiblesProviderTick = nullptr;
	ConVar* CVarEntityProviderTick = nullptr;
	ConVar* CVarObjectiveUpdateTick = nullptr;
	ConVar* CVarWaypointNodeDebugBeamTick = nullptr;
	ConVar* CVarWaypointNodeDebugBeamDist = nullptr;
	ConVar* CVarWaypointNodeDefaultRange = nullptr;
	ConVar* CVarEventProviderTick = nullptr;
	ConVar* CVarBotMaxVisibleDist = nullptr;
	ConVar* CVarBotEnableVisibility = nullptr;
	ConVar* CVarBotEnableBrain = nullptr;
	ConVar* CVarBotMedTargetDistance = nullptr;
	ConVar* CVarBotMovementIgnoreRadius = nullptr;
};

extern ConVarHolder* _ConVarHolder;