#include "Bot.h"
#include "BotTask.h"
#include "BotVisibles.h"
#include "Util.h"

Vector _BotCurrentPos;
Vector _BotEarPos;
TFTeam _BotTeam;

Vector _BotTargetPos;
Vector _BotTargetLookAt;
bool _IsBotViewAngleOverriden;
int _BotPressedButtons;

bool BotTask::OnThink(Bot *bot)
{
	_BotCurrentPos = bot->GetPos();
	_BotEarPos = bot->GetEarPos();
	_BotTeam = bot->GetTeam();

	_BotTargetPos.Zero();
	_BotTargetLookAt.Zero();
	_IsBotViewAngleOverriden = false;
	_BotPressedButtons = 0;

	bool taskResult = _OnThink();

	if (!_IsBotViewAngleOverriden)
	{
		BotVisibleTarget *enemyTarget = bot->GetBotVisibles()->GetMostImportantTarget();
		if (enemyTarget)
		{
			_BotTargetLookAt = enemyTarget->Pos;
			_BotPressedButtons |= IN_ATTACK;
		}
	}
	bot->SetViewAngle(Util::GetLookAtAngleForPos(bot, _BotTargetLookAt));
	bot->SetMovement(Util::GetIdealMoveSpeedsToPos(bot, _BotTargetPos));
	bot->SetPressedButtons(_BotPressedButtons);

	return taskResult;
}

void BotTask::_BotMoveTo(Vector pos)
{
	_BotTargetPos = pos;
}

void BotTask::_SetBotLookAt(Vector pos)
{
	_BotTargetLookAt = pos;
}

void BotTask::_OverrideBotViewAngle()
{
	_IsBotViewAngleOverriden = true;
}

void BotTask::_AddBotPressedButton(int button)
{
	_BotPressedButtons |= button;
}

Vector BotTask::_GetBotPos() const
{
	return _BotCurrentPos;
}

Vector BotTask::_GetBotEarPos() const
{
	return _BotEarPos;
}

TFTeam BotTask::_GetBotTeam() const
{
	return _BotTeam;
}