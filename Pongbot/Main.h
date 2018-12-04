#pragma once
#include <metamod/ISmmPlugin.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <hlsdk/public/eiface.h>
#include <hlsdk/game/shared/IEffects.h>
#include <vector>

class GameFramable;

class Main : public ISmmPlugin {
public:
	bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late);
	bool Unload(char *error, size_t len);
public:
	const char *GetAuthor();
	const char *GetName();
	const char *GetDescription();
	const char *GetURL();
	const char *GetLicense();
	const char *GetVersion();
	const char *GetDate();
	const char *GetLogTag();
public:
	void RegisterGameFramable(GameFramable *framable);
	void UnregisterGameFramable(GameFramable *framable);
private:
	std::vector<GameFramable*> _GameFramables;
// Hooks
private:
	void _OnGameFrame(bool simulation);
};

extern Main _Main;
extern IVEngineServer *Engine;
extern IBotManager *IIBotManager;
extern IServerGameDLL *Server;
extern IPlayerInfoManager *IIPlayerInfoManager;
extern IServerPluginHelpers *IIServerPluginHelpers;
extern IServerGameClients *IIServerGameClients;
extern IEffects *IIEffects;

PLUGIN_GLOBALVARS();