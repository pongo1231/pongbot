#pragma once
#include "BotTask.h"

struct edict_t;

class BotTaskMeleeCombat : public BotTask
{
public:
	BotTaskMeleeCombat(Bot *bot, edict_t *targetEdict);

private:
	edict_t *_TargetEdict;
	IPlayerInfo *_TargetPlayerInfo;

	virtual bool _OnThink();
};