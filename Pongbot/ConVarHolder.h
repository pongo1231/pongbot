#pragma once
#include <hlsdk/public/tier1/convar.h>

class ConVarHolder
{
private:
	ConVarHolder()
	{}

public:
	static void Init();
	static void Destroy();

	ConVar *CVarBotAimSensivity;
	ConVar *CVarBotNodeTouchRadius;
	ConVar *CVarBotPosStuckPanicTime;
	ConVar *CVarBotTargetPosDebugBeamTick;
	ConVar *CVarBotPrimaryWeaponDist;
	ConVar *CVarBotSecondaryWeaponDist;
	ConVar *CVarBotBehaviourUpdateTick;
	ConVar *CVarBotVisibilityTick;
	ConVar *CVarVisiblesProviderTick;
	ConVar *CVarEntityProviderTick;
	ConVar *CVarObjectiveUpdateTick;
	ConVar *CVarWaypointNodeDebugBeamTick;
	ConVar *CVarWaypointNodeDebugBeamDist;
};

extern ConVarHolder *_ConVarHolder;