#include "stdafx.h"
#include "IConVarBase.h"
#include <metamod/ISmmPlugin.h>

extern ISmmAPI* g_SMAPI;
extern ISmmPlugin* g_PLAPI;

IConVarBase::IConVarBase()
{
	g_pCVar = (ICvar*)((g_SMAPI->GetEngineFactory())(CVAR_INTERFACE_VERSION, nullptr));
	ConVar_Register(0, this);
}

bool IConVarBase::RegisterConCommandBase(ConCommandBase* cVar)
{
	return META_REGCVAR(cVar);
}