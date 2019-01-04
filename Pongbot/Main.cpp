#include "Main.h"
#include "Info.h"
#include "BotManager.h"
#include "WaypointManager.h"
#include "IGameFramable.h"
#include "TraceHeaders.h"
#include "EntityProvider.h"
#include "TFClassInfoProvider.h"
#include "ObjectivesProvider.h"
#include <hlsdk/game/shared/IEffects.h>
#include <hlsdk/public/eiface.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <cstdlib>

Main _Main;
IVEngineServer *Engine;
IBotManager *IIBotManager;
IServerGameDLL *Server;
IPlayerInfoManager *IIPlayerInfoManager;
IServerPluginHelpers *IIServerPluginHelpers;
IServerGameClients *IIServerGameClients;
IEffects *IIEffects;
IEngineTrace *IIEngineTrace;

std::vector<IGameFramable*> _IGameFramables;

PLUGIN_EXPOSE(Main, _Main);
SH_DECL_HOOK1_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool);

bool Main::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	GET_V_IFACE_CURRENT(GetEngineFactory, Engine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
	GET_V_IFACE_CURRENT(GetServerFactory, IIBotManager, IBotManager, INTERFACEVERSION_PLAYERBOTMANAGER);
	GET_V_IFACE_CURRENT(GetServerFactory, Server, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_CURRENT(GetServerFactory, IIPlayerInfoManager, IPlayerInfoManager, INTERFACEVERSION_PLAYERINFOMANAGER);
	GET_V_IFACE_CURRENT(GetEngineFactory, IIServerPluginHelpers, IServerPluginHelpers, INTERFACEVERSION_ISERVERPLUGINHELPERS);
	GET_V_IFACE_CURRENT(GetServerFactory, IIServerGameClients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS);
	GET_V_IFACE_CURRENT(GetServerFactory, IIEffects, IEffects, IEFFECTS_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, IIEngineTrace, IEngineTrace, INTERFACEVERSION_ENGINETRACE_SERVER);

	SH_ADD_HOOK(IServerGameDLL, GameFrame, Server, SH_MEMBER(this, &Main::_OnGameFrame), true);

	BotManager::Init();
	WaypointManager::Init();
	EntityProvider::Init();
	TFClassInfoProvider::Init();
	ObjectivesProvider::Init();

	return true;
}

bool Main::Unload(char *error, size_t len)
{
	BotManager::Destroy();
	WaypointManager::Destroy();
	EntityProvider::Destroy();
	TFClassInfoProvider::Destroy();
	ObjectivesProvider::Destroy();

	SH_REMOVE_HOOK(IServerGameDLL, GameFrame, Server, SH_MEMBER(this, &Main::_OnGameFrame), true);

	return true;
}

const char *Main::GetAuthor()
{
	return Info::Author;
}

const char *Main::GetName()
{
	return Info::Name;
}

const char *Main::GetDescription()
{
	return Info::Description;
}

const char *Main::GetURL()
{
	return Info::URL;
}

const char *Main::GetLicense()
{
	return Info::License;
}

const char *Main::GetVersion()
{
	return Info::Version;
}

const char *Main::GetDate()
{
	return Info::Date;
}

const char *Main::GetLogTag()
{
	return Info::Name;
}

void Main::RegisterIGameFramable(IGameFramable *framable)
{
	// Check if not already registered
	for (IGameFramable *registeredFramable : _IGameFramables)
		if (registeredFramable == framable)
			return;
	_IGameFramables.push_back(framable);
}

void Main::UnregisterIGameFramable(IGameFramable *framable)
{
	for (uint8_t i = 0; i < _IGameFramables.size(); i++)
		if (_IGameFramables[i] == framable)
			_IGameFramables.erase(_IGameFramables.begin() + i);
}

void Main::_OnGameFrame(bool simulation)
{
	for (IGameFramable *framable : _IGameFramables)
		framable->OnGameFrame();
}