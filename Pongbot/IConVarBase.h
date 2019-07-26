#pragma once
#include <hlsdk/public/tier1/convar.h>

class IConVarBase : public IConCommandBaseAccessor
{
public:
	IConVarBase();

private:
	bool RegisterConCommandBase(ConCommandBase* pVar);
};