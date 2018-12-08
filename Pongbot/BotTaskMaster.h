#pragma once

class Bot;
class Vector2D;
class QAngle;

class BotTaskMaster {
public:
	BotTaskMaster(Bot *bot);
	~BotTaskMaster();

public:
	void UpdateClass();
	virtual void OnThink(int *&pressedButtons, Vector2D *&movement, QAngle *&lookAt);

protected:
	Bot *_Bot;

private:
	BotTaskMaster *_BotClassTaskMaster;
};

