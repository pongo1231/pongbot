#include "Bot.h"
#include "BotTask.h"
#include "BotVisibles.h"
#include "Util.h"

Vector _BotCurrentPos;
Vector _BotTargetPos;
QAngle _BotViewAngle;
bool _IsBotViewAngleOverriden;
int _BotPressedButtons;
TFTeam _BotTeam;

bool BotTask::OnThink(Bot *bot)
{
	_BotCurrentPos = bot->GetPos();
	_IsBotViewAngleOverriden = false;
	_BotPressedButtons = 0;
	_BotTeam = bot->GetTeam();

	bool taskResult = _OnThink();

	bot->SetMovement(Util::GetIdealMoveSpeedsToPos(bot, _BotTargetPos));
	QAngle finalViewAngle = _BotViewAngle;
	if (!_IsBotViewAngleOverriden)
	{
		BotVisibleTarget *enemyTarget = bot->GetBotVisibles()->GetMostImportantTarget();
		if (enemyTarget)
		{
			finalViewAngle = Util::GetLookAtAngleForPos(bot, enemyTarget->Pos);
			_BotPressedButtons |= IN_ATTACK;
		}
	}
	bot->SetViewAngle(finalViewAngle);
	bot->SetPressedButtons(_BotPressedButtons);

	return taskResult;
}

void BotTask::_BotMoveTo(Vector pos)
{
	_BotTargetPos = pos;
}

QAngle BotTask::_SetBotViewAngle(QAngle angle)
{
	_BotViewAngle = angle;
}

void BotTask::_OverrideBotViewAngle(bool override)
{
	_IsBotViewAngleOverriden = override;
}

void BotTask::_AddBotPressedButton(int button)
{
	_BotPressedButtons |= button;
}

Vector BotTask::_GetBotPos() const
{
	return _BotCurrentPos;
}

TFTeam BotTask::_GetBotTeam() const
{
	return _BotTeam;
}