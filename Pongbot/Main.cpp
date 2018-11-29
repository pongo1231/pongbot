#include "Main.h"
#include "Info.h"
#include "BotManager.h"

Main _Main;
IVEngineServer *Engine;
IBotManager *IIBotManager;
IServerGameDLL *Server;
IPlayerInfoManager *IIPlayerInfoManager;
IServerPluginHelpers *IIServerPluginHelpers;

PLUGIN_EXPOSE(Main, _Main);

bool Main::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late) {
	PLUGIN_SAVEVARS();
	GET_V_IFACE_CURRENT(GetEngineFactory, Engine, IVEngineServer, INTERFACEVERSION_VENGINESERVER);
	GET_V_IFACE_CURRENT(GetServerFactory, IIBotManager, IBotManager, INTERFACEVERSION_PLAYERBOTMANAGER);
	GET_V_IFACE_CURRENT(GetServerFactory, Server, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_CURRENT(GetServerFactory, IIPlayerInfoManager, IPlayerInfoManager, INTERFACEVERSION_PLAYERINFOMANAGER);
	GET_V_IFACE_CURRENT(GetEngineFactory, IIServerPluginHelpers, IServerPluginHelpers, INTERFACEVERSION_ISERVERPLUGINHELPERS);

	BotManager::Init();
	return true;
}

bool Main::Unload(char *error, size_t len) {
	BotManager::Destroy();
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