#pragma once
#include "../Bot.h"
#include "Tasks/BotTask.h"
#include "../../TF2/Events/IEventHooker.h"

enum BotState
{
// TODO: Signal stuff like needing ammo or so here
};

class Player;

class BotBrain : public IEventHooker
{
public:
	BotBrain(Bot* bot) : _ABot(bot), _BotTask(nullptr),  _ThinkTime(0.f), _States(0) {} /* To invoke OnSpawn() */

public:
	void OnThink();

	virtual void OnSpawn(Player player);
	virtual void OnObjectiveUpdate();

protected:
	Bot* _GetBot() const;
	void _SetBotTask(BotTask* task);
	bool _HasBotTask() const;
	bool _IsCurrentBotTaskOfType(const std::type_info& type) const;
	void _AddState(BotState state);
	void _RemoveState(BotState state);
	bool _HasState(BotState state) const;

private:
	Bot* _ABot;
	BotTask* _BotTask;
	float _ThinkTime;
	unsigned int _States;

	void _DefaultThink();
	void _ClearTask();
	void _ResetState();

	virtual void _OnThink() = 0;
	virtual void _OnSpawn() {};
};