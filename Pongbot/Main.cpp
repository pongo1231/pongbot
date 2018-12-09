#include "Main.h"
#include "Info.h"
#include "BotManager.h"
#include "WaypointManager.h"
#include "GameFramable.h"
#include "TraceHeaders.h"
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

std::vector<GameFramable*> _GameFramables;

PLUGIN_EXPOSE(Main, _Main);
SH_DECL_HOOK1_void(IServerGameDLL, GameFrame, SH_NOATTRIB, 0, bool);

bool Main::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late) {
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
	return true;
}

bool Main::Unload(char *error, size_t len) {
	BotManager::Destroy();
	WaypointManager::Destroy();

	SH_REMOVE_HOOK(IServerGameDLL, GameFrame, Server, SH_MEMBER(this, &Main::_OnGameFrame), true);
	return true;
}

const char *Main::GetAuthor() {
	return Info::Author;
}

const char *Main::GetName() {
	return Info::Name;
}

const char *Main::GetDescription() {
	return Info::Description;
}

const char *Main::GetURL() {
	return Info::URL;
}

const char *Main::GetLicense() {
	return Info::License;
}

const char *Main::GetVersion() {
	return Info::Version;
}

const char *Main::GetDate() {
	return Info::Date;
}

const char *Main::GetLogTag() {
	return Info::Name;
}

void Main::RegisterGameFramable(GameFramable *framable) {
	// Check if not already registered
	for (GameFramable *registeredFramable : _GameFramables)
		if (registeredFramable == framable)
			return;
	_GameFramables.push_back(framable);
}

void Main::UnregisterGameFramable(GameFramable *framable) {
	for (uint8_t i = 0; i < _GameFramables.size(); i++)
		if (_GameFramables[i] == framable)
			_GameFramables.erase(_GameFramables.begin() + i);
}

void Main::_OnGameFrame(bool simulation) {
	for (GameFramable *framable : _GameFramables)
		framable->OnGameFrame();
}