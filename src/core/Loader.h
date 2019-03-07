/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __LOADER_H__
#define __LOADER_H__


#include "../../sdk/rmxBase.h"
#include "../../sdk/rmxList.h"
#include "../../sdk/Win32/Timer.h"

#include "PluginTree.h"



class vPluginLoader;
class vPluginCallback;
class rmxMasterPlugin;
class vSqlite;
class rmx_server;
extern vPluginLoader *plg;


//todo:  this entire class has way too much bloat in it.
//       the new primary function of this class is simply
//       to manage a list of plugins, nothing more


class vPluginLoader : public vTimer {
  public:
    vPluginLoader();
    virtual ~vPluginLoader();

  protected:
    virtual void onInit();
    virtual void onQuit();

    virtual BOOL onRmxPostInit();
    virtual void onRmxPostLoad() {}
    virtual void initMasterPlugin(rmxMasterPlugin *master);
    virtual void initMasterLists( rmxMasterPlugin *master) {}

    virtual void onTimer(DWORD systemTime);

  public:
	  inline vSqlite *sql() const { return sqlite; }
	  const char *getSetting(const char *key) const;


    HPLUGIN loadPlugin(const char *filename);
    virtual vPlugin *loadPlugin(const char *path, HMODULE hMod);
    BOOL unloadPlugin(HPLUGIN plugin);



    inline BOOL isLoaded() const { return loaded; }

    void scanForPlugins();

    inline vPlugin *getFirstPlugin()          const { return plugins.getFirstItem(); }
    inline vPlugin *getLastPlugin()           const { return plugins.getLastItem();  }
    inline vPlugin *getNextPlugin(vPlugin* p) const { return plugins.getNextItem(p); }
    inline vPlugin *getPrevPlugin(vPlugin* p) const { return plugins.getPrevItem(p); }

    inline vPlugin *getPlugin(HPLUGIN p) const { return (plugins.hasItem((vPlugin*)p)) ? ((vPlugin*)p) : (NULL); }

    vPlugin *getPlugin(const char *name) const;
    vPlugin *getPluginById(RMXLONG id) const;
    vPlugin *getPluginByFilename(const char *filename) const;

    inline rmxMasterPlugin *getMasterPlugin() const { return master; }
    HPLUGIN getMasterHandle() const;

    inline void setMasterPlugin(rmxMasterPlugin *plugin) { master = plugin; }

    BOOL isMaster(const vPlugin *plugin);
    
    inline BOOL hasPlugin(const vPlugin *plugin) const { return plugins.hasItem((vPlugin*)plugin); }
    inline BOOL hasPlugin(HPLUGIN        plugin) const { return plugins.hasItem((vPlugin*)plugin); }
    inline BOOL hasPlugin(const char  *filename) const { return (getPluginByFilename(filename) != NULL); }

    void removePlugin(HMODULE hInst);


    cfgTree *getPluginTree();

    void makeDirectories();

//	int getPluginRootId(vPlugin *plugin, BOOL add=FALSE);

    void appendPlugin(vPlugin *plugin);
    void removePlugin(vPlugin *plugin);

    inline void appendCallback(vPluginCallback *cb) { callbacks.appendItem(cb); }
    inline void removeCallback(vPluginCallback *cb) { callbacks.removeItem(cb); }


    void destroy();

private:
    void destroyRmxCore();


  private:
    rmxList<vPlugin*> plugins;
    rmxList<vPluginCallback*> callbacks;

    BOOL loaded;
    UINT starttime;

    rmxMasterPlugin *master;
    vPluginTree *pluginTree;

  protected:
	vSqlite *sqlite;
	rmx_server *webserver;
};


#endif //__LOADER_H__
