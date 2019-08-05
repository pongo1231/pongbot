#pragma once
#include "../Bot.h"
#include "Tasks/BotTask.h"
#include "../../TF2/Events/IEventHooker.h"

enum BotState
{
// TODO: Signal stuff like needing ammo or so here
};

class BotBrain : public IEventHooker
{
public:
	BotBrain(Bot* bot) : _ABot(bot), _BotTask(nullptr), _IsBotDead(true) {} /* To invoke OnSpawn() */

public:
	void OnThink();
	void OnSpawn();

	virtual void OnObjectiveUpdate();

protected:
	Bot* _GetBot() const;
	void _SetBotTask(BotTask* task);
	bool _HasBotTask() const;
	void _AddState(BotState state);
	void _RemoveState(BotState state);
	bool _HasState(BotState state) const;

private:
	Bot* _ABot;
	BotTask* _BotTask;
	float _ThinkTime;
	unsigned int _States;
	bool _IsBotDead;
	bool _IsBotInMeleeFight;

	void _DefaultThink();
	void _ClearTask();
	void _ResetState();

	virtual void _OnThink() = 0;
	virtual void _OnSpawn() = 0;
};

