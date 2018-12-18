#pragma once
#include <hlsdk/public/tier1/convar.h>

class ConVarBase : public IConCommandBaseAccessor
{
public:
	ConVarBase();

private:
	bool RegisterConCommandBase(ConCommandBase *pVar);
};