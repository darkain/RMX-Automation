/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../../sdk/Win32/Win32.h"

#include "Loader.h"
#include "Plugin.h"
#include "PluginTree.h"
#include "xml/xmlSettings.h"
#include "Callbacks/cbConsole.h"


vPluginTree::vPluginTree(cfgBase *parent) : cfgTree("Plugins", parent) {

#ifdef RMX_PLUGINLOADER
  new vPluginScanBtn(this);

  { //TODO: revalidate this after JSON output is properly escaped in strings > \\ instead of \ -
    char path[MAX_PATH]="";
    VPRINTF(path, sizeof(path), "%s\\rmx_plugins\\", RMXOS::getPath());
//    cfgLink *link = new cfgLink("rmx_plugins", this);
//    link->setUrl(path);
//    link->setSavable(FALSE);
  }
#endif //RMX_PLUGINLOADER

  new cfgLine(this);
}


vPluginTree::~vPluginTree() {
}


cfgBase *vPluginTree::onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist) {
  if (VSTRCMP(type, "bool") == 0) {
    const char *filename = paramlist->getParam("filename");
    if (filename) {
      cfgBase *list = addPlugin(filename, name);
      if (list) list->setVisible(FALSE);
      return list;
    }
  }
  return cfgTree::onNewExternalChild(name, type, paramlist);
}


vPluginManager *vPluginTree::addPlugin(const char *filename, const char *name) {
  vPluginManager *b = getManager(filename);
  if (b) return NULL;

  b = new vPluginManager(name, filename, this);
  managers.appendItem(b);
  return b;
}


vPluginManager *vPluginTree::getManager(const char *filename) {
  if (!filename) return NULL;

  vPluginManager *m = managers.getFirstItem();
  while (m) {
    const char *fn = m->getFilename();
    if (fn) {
      if (!VSTRCMP(fn, filename)) return m;
    }
    m = managers.getNextItem(m);
  }

  return NULL;
}


vPluginManager *vPluginTree::getManager(vPlugin *plugin) {
  vPluginManager *m = managers.getFirstItem();
  while (m) {
    if (m->getPlugin() == plugin) return m;
    m = managers.getNextItem(m);
  }
  return NULL;
}


void vPluginTree::loadAll(BOOL init) {
  rmxList<vPlugin*> newplugins;

  vPluginManager *m = managers.getFirstItem();
  while (m) {
    if (m->load()) newplugins.appendItem(m->getPlugin());
    m = managers.getNextItem(m);
  }

  if (init) {
    vPlugin *plugin = newplugins.getFirstItem();
    while (plugin) {
      plugin->onInitPlugin();
      plugin = newplugins.getNextItem(plugin);
    }

	plugin = newplugins.getFirstItem();
    while (plugin) {
      plugin->onPostInitPlugin();               //TODO: find out why there are multiple locations for plugin init stuffs
      plugin = newplugins.getNextItem(plugin);
    }
  }
}


void vPluginTree::unloadAll() {
  vPluginManager *m = managers.getFirstItem();
  while (m) {
    m->unload();
    m = managers.getNextItem(m);
  }
}



//------------------------------------------------------------------------------


vPluginManager::vPluginManager(const char *name, const char *file, vPluginTree *parent) : cfgBool(name, parent, NULL) {
	setParam("filename", file);

	pathname = new cfgLabel("plugin_path",  this); 
	sdkBuild = new cfgLabel("plugin_sdk",   this);
	libBuild = new cfgLabel("plugin_build", this);

	pathname->setLabel(file)->setSavable(FALSE);
	sdkBuild->setLabel("Unknown SDK Version")->setSavable(FALSE);
	libBuild->setLabel("Unknown LIB Version")->setSavable(FALSE);

	plugin = NULL;
	setValue(TRUE);
	setExpanded(TRUE);
}



vPluginManager::~vPluginManager() {
	unload();
}



BOOL vPluginManager::load() {
#ifdef RMX_PLUGINLOADER

  //build the path to the plugin
  char path[MAX_PATH] = "";
  VPRINTF(path, sizeof(path), "%s\\rmx_plugins\\%s", RMXOS::getPath(), getFilename());
  if (!VFEXISTS(path)) {
    setVisible(FALSE);
    return FALSE;
  }

  setVisible(TRUE);
  if (plugin) return FALSE;
  if (!getValue()) return FALSE;

  //load the DLL file
  SetErrorMode(SEM_FAILCRITICALERRORS);
  HMODULE hMod = NULL;
  try {
     hMod = LoadLibraryA(path);
  } catch(...) {
    hMod = NULL;
  }
  SetErrorMode(0);

  if (!hMod) {
    setVisible(FALSE);
    return failedMessage("LoadLibrary");
  }


  //check MSDN notes on why i call this
  DisableThreadLibraryCalls(hMod);


  //try to initialize the plugin
  plugin = plg->loadPlugin(path, hMod);
  if (!plugin) {
    FreeLibrary(hMod);
    return failedMessage("LoadPlugin");
  }

  if (plugin->getStatus() == 0) {
    FreeLibrary(hMod);
    vdelnull(plugin);
    return failedMessage("PluginStatus");
  }

  plg->appendPlugin(plugin);
  char str[MAX_PATH];
  VPRINTF(str, sizeof(str), "Plugin Loaded Successfully: %s", getFilename());
  CONSOLE_BROADCAST->print("Core", str);

  setName(plugin->getName());


  if (plugin->getSdkVersion() > 0) {
    VPRINTF(str, sizeof(str), "SDK Build: %d", plugin->getSdkVersion());
    sdkBuild->setName(str);
  } else {
    sdkBuild->setName("Unknown SDK Build");
  }

  if (plugin->getLibVersion() > 0) {
    VPRINTF(str, sizeof(str), "LIB Build: %d", plugin->getLibVersion());
    libBuild->setName(str);
  } else {
    libBuild->setName("Unknown LIB Build");
  }

  setVisible(TRUE);
  return TRUE;

#else //RMX_PLUGINLOADER

	return FALSE;

#endif //RMX_PLUGINLOADER
} 


void vPluginManager::unload() {
	if (!plugin) return;

	//  plugin->onPreSaveSettings();
	//  plugin->saveSettings();
	//  plugin->onSaveSettings();

	delete plugin;
	plugin = NULL;
}

/*
void vPluginManager::onSetUserData(const prmBase *userdata) {
  cfgBool::onSetUserData(userdata);
*/

void vPluginManager::cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert) {
  if (param != this  ||  message != CB_PARAM_UPDATED  ||  !plg->isLoaded()) return;

  if (getValueInt()) {
    if (load()) if (plugin) {
      plugin->onInitPlugin();

//      plugin->onPreLoadSettings();
//      xmlSetting loadxml(plugin);
//      plugin->onLoadSettings();

//      plugin->assignIds();

      plugin->onPostInitPlugin();
    }
  } else {
    unload();
  }
}

/*
void vPluginManager::onPreLoadSettings() {
//  setReadOnly(TRUE);
  cfgBool::onPreLoadSettings();
}
*/
/*
void vPluginManager::onLoadSettings() {
  cfgBool::onLoadSettings();
//  setReadOnly(FALSE);
}
*/

BOOL vPluginManager::failedMessage(const char *message) {
	char str[MAX_PATH];
	VPRINTF(str, sizeof(str), "Error loading plugin: %s - Reason: %s", getFilename(), message);
	CONSOLE_BROADCAST->printError("Core", str);
	return FALSE;
}


//------------------------------------------------------------------------------


vPluginScanBtn::vPluginScanBtn(vPluginTree *parent) : cfgButton("Scan for new plugins", parent) {
	setButtonText("Scan");
	setSavable(FALSE);
	par = parent;
}


void vPluginScanBtn::onClick(HPLUGIN source) {
	cfgButton::onClick(source);
	plg->scanForPlugins();
	par->loadAll(TRUE);
}
