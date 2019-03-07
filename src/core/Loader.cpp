/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../../sdk/rmxString.h"
#include "../../sdk/cfg/config.h"

#include "Loader.h"
#include "Plugin.h"
#include "RmxPlugin.h"
#include "rmxMasterPlugin.h"
#include "PluginTree.h"

#include "callbacks/cbPlugin.h"
#include "callbacks/cbConsole.h"
#include "callbacks/cbFunction.h"

#include "xml/xmlSettings.h"

#include "lua/luaHelper.h"
#include "lua/luaFunction.h"
#include "lua/luaConfig.h"

#include "json/jsonParser.h"

#include "sqlite/sqlite.h"
#include "sqlite/sqlPluginCallback.h"

#include "tcp/http.h"
#include "../../libraries/jnetlib/util.h"

#include "Resource.h"
#include "funcNone.h"
#include "coreSettings.h"

#include <io.h>



vPluginLoader *plg = NULL;
extern RMX_ENTRY rmx;



vPluginLoader::vPluginLoader() : vTimer(10, FALSE) {
  starttime = RMXOS::getTick();
  srand(starttime);

  makeDirectories();

  rmx = _RMX_CORE;

  loaded     = FALSE;
  master     = NULL;
  pluginTree = NULL;
  plg        = this;
  sqlite     = NULL;

  
#ifdef _RMX_SQLITE_
  sqlite = new vSqlite();

  sqlite->beginTransaction();

  sqlite->setSetting("_sqlite_version",        sqlite3_libversion());
  sqlite->setSetting("_sqlite_version_number", sqlite3_libversion_number());
  sqlite->setSetting("_sqlite_source_id",      sqlite3_sourceid());

  sqlite->setSetting("_rmx_version", _RMX_VERSION);
  sqlite->setSetting("_app_version", _APP_VERSION);
  sqlite->setSetting("_sdk_version", _SDK_VERSION);

  char str[128] = "";
  VPRINTF(str, sizeof(str), "%d.%d.%d", RMXOS::getOsVersionMajor(), RMXOS::getOsVersionMinor(), RMXOS::getOsVersionBuild());
  sqlite->setSetting("_rmx_windows",  str);
  sqlite->setSetting("_rmx_platform", RMXOS::getOsVersionPlatform());

#ifdef _M_X64
  sqlite->setSetting("_rmx_arch", "x64");
#else //_M_X64
  sqlite->setSetting("_rmx_arch", "x86");
#endif //_M_X64

  sqlite->setMimeType("htm",  "text/html");
  sqlite->setMimeType("html", "text/html");
  sqlite->setMimeType("xml",  "text/xml");
  sqlite->setMimeType("css",  "text/css");
  sqlite->setMimeType("js",   "application/javascript");
  sqlite->setMimeType("jpg",  "image/jpeg");
  sqlite->setMimeType("jpeg", "image/jpeg");
  sqlite->setMimeType("gif",  "image/gif");
  sqlite->setMimeType("png",  "image/png");
  sqlite->setMimeType("bmp",  "image/x-windows-bmp");
  sqlite->setMimeType("ico",  "image/x-icon");

  sqlite->commitTransaction();

#endif


#ifdef _RMX_LUA_
  lua_create();
#endif //_RMX_LUA_

  JNL::open_socketlib();
  webserver = new rmx_server();
}



vPluginLoader::~vPluginLoader() {
#ifdef _RMX_SQLITE_
  if (sqlite) sqlite->beginTransaction();
#endif //_RMX_SQLITE_

  delete webserver;
  webserver = NULL;

  pluginTree->unloadAll();

  vPluginCallback *cb = callbacks.getFirstItem();
  while (cb) {
    delete cb;
    callbacks.removeItem(cb);
    cb = callbacks.getFirstItem();
  }

  CONSOLE_MAIN->print("Core", "Stopping");

  delete master;
  master = NULL;
  pluginTree = NULL;
  
#ifdef _RMX_LUA_
  lua_destroy();
#endif //_RMX_LUA_

#ifdef _RMX_SQLITE_
  if (sqlite) sqlite->commitTransaction();
  vdelnull(sqlite);
#endif

/*
int sqlite3_set_authorizer(
  sqlite3*,
  int (*xAuth)(void*,int,const char*,const char*,const char*,const char*),
  void *pUserData
);
*/

  plg = NULL;
}



void vPluginLoader::onInit() {
  //initialize master plugin
  initMasterPlugin(master);

  //prepare our plugin tree
  getPluginTree();

  //finalize initialization of master plugin
  initMasterLists(master);
  master->onInitPlugin();
  master->initConfig();


  UINT tick = RMXOS::getTick();

  scanForPlugins();
  pluginTree->loadAll();
  
  vPlugin *plugin = plugins.getFirstItem();
  while (plugin) {
    if (!isMaster(plugin)) {
      plugin->onInitPlugin();
    }
    plugin = plugins.getNextItem(plugin);
  }

  char str[256]="";
  VPRINTF(str, sizeof(str), "Plugins Finished Loading in %dms", RMXOS::getTick()-tick);
  CONSOLE_MAIN->print("Core", str);
  
  startTimer();
}


void vPluginLoader::initMasterPlugin(rmxMasterPlugin *master) {
  extrmxmsg.plugin = master->getHandle();

  new vConsolePluginCallback();
  new vFunctionPluginCallback();
  new sqlPluginCallback();

  setMasterPlugin(master);
  appendPlugin(master);


  new funcNone(master);


  cfgStatusBar *statusbar = new cfgStatusBar(master);
//  new coreLoadSettings(statusbar);
//  new coreSaveSettings(statusbar);


  //about information
  cfgTree *about = new cfgTree("About", master);
  about->setTitle("About RMX Automation");
  about->setSavable(FALSE);

  (new cfgLabel("copyright_rmx_a2",  about))->alignCenter()->setLabel("RMX Automation v2.0 Copyright (C) 2009-2012 Darkain Multimedia");
  (new cfgLabel("copyright_rmx_a1",  about))->alignCenter()->setLabel("RMX Automation v1.x Copyright (C) 2006-2007 Vincent E. Milum Jr.");
  (new cfgLabel("copyright_rmx_gpc", about))->alignCenter()->setLabel("RM-X General Purpose Control Copyright (C) 2001-2006 Vincent E. Milum Jr.");
  (new cfgLabel("copyright_rmx_arr", about))->alignCenter()->setLabel("All rights reserved");

  cfgLink *link = new cfgLink("RMX.SourceForge.Net", about);
  link->setUrl("http://rmx.sourceforge.net/")->alignCenter(); /* ->setTooltip(link->getUrl()); */


#ifdef _RMX_LUA_
  new vLuaFunction(master);
  new vLuaConfigTree(master);
#endif //_RMX_LUA_


  new rmxJsonParser(master);
}



void vPluginLoader::onTimer(DWORD systemTime) {
  stopTimer();

  if (isLoaded()) {

    DEBUGMSG("Core", "DELAYED DELETE");
	this->destroyRmxCore();

  } else {
    CONSOLE_MAIN->print("Core", "Begining Post Init");
    
    if (onRmxPostInit()) {
      CONSOLE_MAIN->print("Core", "Post Init Finished Successfully");
    } else {
      CONSOLE_MAIN->printError("Core", "Error in Post Init");
    }

    char str[32];
    VPRINTF(str, sizeof(str), "Total Start Time: %dms", RMXOS::getTick()-starttime);
    CONSOLE_MAIN->print("Core", str);
  }
}


const char *vPluginLoader::getSetting(const char *key) const {
#ifdef _RMX_SQLITE_
	return sqlite->getSettingString(key);
#else  // _RMX_SQLITE_
	return "[unsupported]";
#endif // _RMX_SQLITE_
}



BOOL vPluginLoader::onRmxPostInit() {

  vPlugin *plugin = plugins.getFirstItem();
  while (plugin) {
    plugin->onPostInitPlugin();
    plugin = plugins.getNextItem(plugin);
  }

  loaded = TRUE;
  onRmxPostLoad();

#ifdef _RMX_LUA_
  lua_start();
#endif //_RMX_LUA_

  return TRUE;
}


void vPluginLoader::onQuit() {
  CONSOLE_MAIN->print("Core", "Unloading Plugins");

#ifdef _RMX_SQLITE_
  sqlite->beginTransaction();
  pluginTree->unloadAll();
  sqlite->commitTransaction();
#else  // _RMX_SQLITE_
  pluginTree->unloadAll();
#endif // _RMX_SQLITE_
}


//----------------------------------------------------------



void vPluginLoader::appendPlugin(vPlugin *plugin) {
  if (!plugin) return;
  if (plugins.hasItem(plugin)) return;

  //Display debug message
  DEBUGMSG("Add Plugin", plugin->getName());


  //Notify callbacks of this new plugin
  plugins.appendItem(plugin);
  vPluginCallback *cb = callbacks.getFirstItem();
  while (cb) {
    cb->onAppendPlugin(plugin);
    cb = callbacks.getNextItem(cb);
  }
}


void vPluginLoader::removePlugin(vPlugin *plugin) {
  if (!plugin) return;

#ifdef RMX_DEBUG
  CONSOLE_MAIN->printDebug("Remove Plugin", plugin->getName());
#endif

  vPluginCallback *cb = callbacks.getFirstItem();
  while (cb) {
    cb->onRemovePlugin(plugin);
    cb = callbacks.getNextItem(cb);
  }

  plugins.removeItem(plugin);

  SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
}



void vPluginLoader::makeDirectories() {
#ifndef _RMX_MEDIA_REMOTE_
  char path[MAX_PATH] = "";

#ifdef RMX_PLUGINLOADER
  VSTRCPY(path, RMXOS::getPath(), sizeof(path));
  VSTRCAT(path, "\\rmx_plugins\\", sizeof(path));
  CreateDirectoryA(path, NULL);
#endif //RMX_PLUGINLOADER

#ifdef _RMX_LUA_
  VSTRCPY(path, RMXOS::getPath(), sizeof(path));
  VSTRCAT(path, "\\rmx_scripts\\", sizeof(path));
  CreateDirectoryA(path, NULL);
#endif //_RMX_LUA_
  
#ifdef _RMX_XML_SETTINGS_
  VSTRCPY(path, RMXOS::getPath(), sizeof(path));
  VSTRCAT(path, "\\rmx_settings\\", sizeof(path));
  CreateDirectoryA(path, NULL);
#endif //_RMX_XML_SETTINGS_

#endif //_RMX_MEDIA_REMOTE_
}


HPLUGIN vPluginLoader::loadPlugin(const char *filename) {
  vPluginManager *manager = pluginTree->getManager(filename);
  if (!manager) manager = pluginTree->addPlugin(filename);

  if (manager  &&  manager->load()) {
    vPlugin *plugin = manager->getPlugin();
    if (!plugin) return NULL;

    plugin->onInitPlugin();
    plugin->onPostInitPlugin();

    return (HPLUGIN)plugin;
  }

  return NULL;
}


vPlugin *vPluginLoader::loadPlugin(const char *path, HMODULE hMod) {
  RmxPlugin *plugin = NULL;
#ifdef RMX_PLUGINLOADER

#ifdef _DEBUG
  SetLastError(0);
#endif

  FARPROC proc = GetProcAddress(hMod, "_RMX_ENTRY");
  if (proc) {
    plugin = new RmxPlugin(path, hMod, (RMX_ENTRY)proc);
    if (!plugin->testPlugin()) vdelnull(plugin);
  }

#ifdef _DEBUG
  else {
    char str[128];
    VPRINTF(str, sizeof(str), "GetProcAddress: %d", GetLastError());
    CONSOLE_MAIN->printError("Core", str);
  }
#endif

#endif
  return plugin;
}


BOOL vPluginLoader::unloadPlugin(HPLUGIN plugin) {
  vPlugin *plug = getPlugin(plugin);
  if (plug->removable()) {
    vPluginManager *manager = pluginTree->getManager(plug);
    if (manager) {
      manager->unload();
      return TRUE;
    }
  }
  return FALSE;
}


void vPluginLoader::removePlugin(HMODULE hInst) {
  vPlugin *p = plugins.getFirstItem();
  while (p) {
    if (p->gethMod() == hInst) {
      removePlugin(p);
      return;
    }
    p = plugins.getNextItem(p);
  }
}




BOOL vPluginLoader::isMaster(const vPlugin *plugin) {
  return (master == static_cast<const vLocalPlugin*>(plugin));
}


vPlugin *vPluginLoader::getPlugin(const char *name) const {
  vPlugin *plugin = plugins.getCachedItem();
  if (plugin) if (VSTRCMP(plugin->getName(), name) == 0) return plugin;

  plugin = plugins.getFirstItem();
  while (plugin) {
    if (VSTRCMP(plugin->getName(), name) == 0) return plugin;
    plugin = plugins.getNextItem(plugin);
  }
  return NULL;
}


vPlugin *vPluginLoader::getPluginById(RMXLONG id) const {
  vPlugin *plugin = plugins.getFirstItem();
  while (plugin) {
    if (plugin->getPluginId() == id) return plugin;
    plugin = plugins.getNextItem(plugin);
  }
  return NULL;
}


vPlugin *vPluginLoader::getPluginByFilename(const char *filename) const {
  vPlugin *plugin = plugins.getFirstItem();
  while (plugin) {
    const char *fn = plugin->getFilename();
    if (fn != NULL  &&  *fn != NULL) {
      if (VSTRCMPI(fn, filename) == 0) return plugin;
    }
    plugin = plugins.getNextItem(plugin);
  }
  return NULL;
}


HPLUGIN vPluginLoader::getMasterHandle() const {
	vPlugin* plugin = getMasterPlugin();
	return (HPLUGIN)plugin;
}



cfgTree *vPluginLoader::getPluginTree() {
  if (!pluginTree) {
    pluginTree = new vPluginTree(getMasterPlugin());
  }
  return pluginTree;
}



void vPluginLoader::scanForPlugins() {
#ifdef RMX_PLUGINLOADER
  CONSOLE_ACTIVE->print("Core", "Loading Plugins");

  char path[MAX_PATH] = "";  //TODO: make this a cfgBase/ID param.  validate to check for '/', '\', '..', etc
  VPRINTF(path, sizeof(path), "%s\\rmx_plugins\\*.dll", RMXOS::getPath());

  _finddata_t data;
  VINT hFile = _findfirst(path, &data);
  while (hFile != -1) {
    pluginTree->addPlugin(data.name);
    if (_findnext(hFile, &data) == -1) break;
  }

  _findclose(hFile);
#endif //RMX_PLUGINLOADER
}



void vPluginLoader::destroy() {
  if (isLoaded()) startTimer();
}


void vPluginLoader::destroyRmxCore() {
  if (!plg) return;
  onQuit();
  delete this;
}


