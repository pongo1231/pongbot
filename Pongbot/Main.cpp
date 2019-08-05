#include <stdafx.h>
#include "Main.h"
#include "Info.h"
#include "Bot/BotManager.h"
#include "Waypoint/WaypointManager.h"
#include "IGameFramable.h"
#include "TF2/Trace/TraceHeaders.h"
#include "TF2/Entity/EntityProvider.h"
#include "TF2/Class/TFClassInfoProvider.h"
#include "TF2/Objectives/ObjectivesProvider.h"
#include "ConVarHolder.h"
#include "TF2/Events/EventHooksProvider.h"
#include "Util.h"
#include <hlsdk/game/shared/IEffects.h>
#include <hlsdk/public/eiface.h>
#include <hlsdk/public/game/server/iplayerinfo.h>
#include <cstdlib>

Main _Main;
IVEngineServer* Engine = nullptr;
IBotManager* IIBotManager = nullptr;
IServerGameDLL* Server = nullptr;
IPlayerInfoManager* IIPlayerInfoManager = nullptr;
IServerPluginHelpers* IIServerPluginHelpers = nullptr;
IServerGameClients* IIServerGameClients = nullptr;
IEffects* IIEffects = nullptr;
IEngineTrace* IIEngineTrace = nullptr;

std::vector<IGameFramable*> _IGameFramables;

PLUGIN_EXPOSE(Main, _Main);
SH_DECL_HOOK1_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool);

bool Main::Load(PluginId id, ISmmAPI* ismm, char* error, size_t maxlen, bool late)
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

	Util::DebugLog("DEBUG MODE IS ON");

	SH_ADD_HOOK(IServerGameDLL, GameFrame, Server, SH_MEMBER(this, &Main::_OnGameFrame), true);

	float initStartTime = Engine->Time();
	Util::Log("Initializing...");

	BotManager::Init();
	WaypointManager::Init();
	EntityProvider::Init();
	TFClassInfoProvider::Init();
	ObjectivesProvider::Init();
	ConVarHolder::Init();
	EventHooksProvider::Init();

	Util::Log("Initializion done! (Took %.2f seconds)", Engine->Time() - initStartTime);
	Util::Log("!!! Don't forget to set sv_quota_stringcmdspersecond to some high value (e.g. 999999) to prevent crashes !!!");

	return true;
}

bool Main::Unload(char* error, size_t len)
{
	float destroyStartTime = Engine->Time();
	Util::Log("Shutting down...");

	BotManager::Destroy();
	WaypointManager::Destroy();
	EntityProvider::Destroy();
	TFClassInfoProvider::Destroy();
	ObjectivesProvider::Destroy();
	ConVarHolder::Destroy();
	EventHooksProvider::Destroy();

	SH_REMOVE_HOOK(IServerGameDLL, GameFrame, Server, SH_MEMBER(this, &Main::_OnGameFrame), true);

	Util::Log("Shutdown successful! (Took %.2f seconds)", Engine->Time() - destroyStartTime);

	return true;
}

const char* Main::GetAuthor()
{
	return Info::Author;
}

const char* Main::GetName()
{
	return Info::Name;
}

const char* Main::GetDescription()
{
	return Info::Description;
}

const char* Main::GetURL()
{
	return Info::URL;
}

const char* Main::GetLicense()
{
	return Info::License;
}

const char* Main::GetVersion()
{
	return Info::Version;
}

const char* Main::GetDate()
{
	return Info::Date;
}

const char* Main::GetLogTag()
{
	return Info::Name;
}

void Main::RegisterIGameFramable(IGameFramable* framable)
{
	// Check if not already registered
	for (IGameFramable* registeredFramable : _IGameFramables)
	{
		if (registeredFramable == framable)
		{
			return;
		}
	}
	_IGameFramables.push_back(framable);
}

void Main::UnregisterIGameFramable(IGameFramable* framable)
{
	for (uint8_t i = 0; i < _IGameFramables.size(); i++)
	{
		if (_IGameFramables[i] == framable)
		{
			_IGameFramables.erase(_IGameFramables.begin() + i);
		}
	}
}

void Main::_OnGameFrame(bool simulation)
{
	for (IGameFramable* framable : _IGameFramables)
	{
		framable->OnGameFrame();
	}
}