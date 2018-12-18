#pragma once
#include <metamod/ISmmPlugin.h>
#include <vector>

class GameFramable;
class IVEngineServer;
class IBotManager;
class IServerGameDLL;
class IPlayerInfoManager;
class IServerPluginHelpers;
class IServerGameClients;
class IEffects;
class IEngineTrace;

class Main : public ISmmPlugin
{
public:
	bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late);
	bool Unload(char *error, size_t len);

	const char *GetAuthor();
	const char *GetName();
	const char *GetDescription();
	const char *GetURL();
	const char *GetLicense();
	const char *GetVersion();
	const char *GetDate();
	const char *GetLogTag();
	void RegisterGameFramable(GameFramable *framable);
	void UnregisterGameFramable(GameFramable *framable);

private:
	std::vector<GameFramable*> _GameFramables;

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
extern IEngineTrace *IIEngineTrace;

PLUGIN_GLOBALVARS();