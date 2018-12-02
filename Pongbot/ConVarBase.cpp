#include "ConVarBase.h"
#include <metamod/ISmmPlugin.h>

extern ISmmAPI *g_SMAPI;
extern ISmmPlugin *g_PLAPI;

ConVarBase::ConVarBase() {
	g_pCVar = (ICvar*)((g_SMAPI->GetEngineFactory())(CVAR_INTERFACE_VERSION, nullptr));
	ConVar_Register(0, this);
}

bool ConVarBase::RegisterConCommandBase(ConCommandBase *cVar) {
	return META_REGCVAR(cVar);
}