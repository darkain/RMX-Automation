/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "luaHelper.h"
#ifdef _RMX_LUA_


#include "luaPlugin.h"
#include "../Loader.h"
#include "../Plugin.h"


void vLuaPlugin::init(LuaObject globals) {
  LuaObject rmx   = globals.CreateTable("rmx");
  LuaObject lists = globals.CreateTable("lists");

//TODO: bring these up to date with current standards
  rmx.Register("getFirstPlugin",      _getFirstPlugin);
  rmx.Register("getLastPlugin",       _getLastPlugin);
  rmx.Register("getPrevPlugin",       _getPrevPlugin);
  rmx.Register("getNextPlugin",       _getNextPlugin);
  rmx.Register("getPlugin",           _getPlugin);
  rmx.Register("getPluginByFilename", _getPluginByFilename);


  LuaObject plugins = globals.CreateTable("plugins");
  plugins.Register("getVersion",      _getPluginVersion);
  plugins.Register("getSdkVersion",   _getPluginSdkVersion);
  plugins.Register("getLibVersion",   _getPluginLibVersion);
  plugins.Register("getName",         _getPluginName);
  plugins.Register("getFilename",     _getPluginFilename);


  //get lists from plugin
  plugins.Register("getListFirst",    _root_getFirstChild);
  plugins.Register("getListLast",     _root_getLastChild);
  plugins.Register("getListPrev",     _root_getPrevChild);
  plugins.Register("getListNext",     _root_getNextChild);
  plugins.Register("getList",         _root_getList);
  plugins.Register("getRoot",         _root_getRoot);
  plugins.Register("getIdent",        _root_getIdent);
  plugins.Register("hasList",         _root_hasList);


  //primary list information
  lists.Register("getName",           _list_getName);
  lists.Register("setName",           _list_setName);
  lists.Register("getType",           _list_getType);
  lists.Register("message",           _list_message);

  //list user data
  lists.Register("getUserData",       _list_getUserData);
  lists.Register("setUserData",       _list_setUserData);

  //list flags
  lists.Register("getFlags",          _list_getFlags);
  lists.Register("getSecurity",       _list_getSecurity);
  lists.Register("getSecurityEx",     _list_getSecurityEx);



  //list attributes
  lists.Register("isVisible",              _list_isVisible);
  lists.Register("setVisible",             _list_setVisible);
  lists.Register("hasChildrenVisible",     _list_hasChildrenVisible);

  lists.Register("isEnabled",              _list_isEnabled);
  lists.Register("setEnabled",             _list_setEnabled);
  lists.Register("hasChildrenEnabled",     _list_hasChildrenEnabled);

  lists.Register("isHighlighted",          _list_isHighlighted);
  lists.Register("setHighlighted",         _list_setHighlighted);
  lists.Register("hasChildrenHighlighted", _list_hasChildrenHighlighted);
  
  lists.Register("isExpanded",             _list_isExpanded);
  lists.Register("setExpanded",            _list_setExpanded);
  lists.Register("hasChildrenExpanded",    _list_hasChildrenExpanded);
  
  lists.Register("isSavable",              _list_isSavable);
  lists.Register("setSavable",             _list_setSavable);
  lists.Register("hasChildrenSavable",     _list_hasChildrenSavable);
  


  //get and set param value from a list
  lists.Register("getParam",          _list_getParam);
  lists.Register("setParam",          _list_setParam);
  lists.Register("hasParam",          _list_hasParam);
  lists.Register("hasParams",         _list_hasParams);

  lists.Register("insertParam",       _list_insertParam);
  lists.Register("insertParamNoSave", _list_insertParamNoSave);
  lists.Register("getParamCount"    , _list_getParamCount);
  lists.Register("getParamFlags",     _list_getParamFlags);
  lists.Register("isParamSavable",    _list_isParamSavable);
  lists.Register("removeParam",       _list_removeParam);


  //list parents/children
  lists.Register("getParent",         _list_getParent);
  lists.Register("getChild",          _list_getChild);
  lists.Register("getCHildById",      _list_getChildById);
  lists.Register("hasChild",          _list_hasChild);
  lists.Register("hasChildrenChild",  _list_hasChildren);
  lists.Register("getChildCount",     _list_getChildCount);

  lists.Register("getFirstChild",     _list_getFirstChild);
  lists.Register("getLastChild",      _list_getLastChild);
  lists.Register("getPrevChild",      _list_getPrevChild);
  lists.Register("getNextChild",      _list_getNextChild);
}




vPlugin *vLuaPlugin::getPlugin(LuaState *lua) {
  if (lua->GetTop() > 0) {
    if (lua->Stack(1).IsLightUserData()) {
      vPlugin *plugin = (vPlugin*) lua->Stack(1).GetLightUserData();
      if (plg->hasPlugin(plugin)) return plugin;
    }
  }
  return NULL;
}


HLIST vLuaPlugin::getList(vPlugin *plugin, LuaState *lua, int pos, BOOL verify) {
  if (!plugin) return NULL;
  if (lua->GetTop() >= pos) {
    if (lua->Stack(pos).IsLightUserData()) {
      HLIST list = (HLIST) lua->Stack(pos).GetLightUserData();
      if (verify) {
        if (plugin->root_hasList(list, TRUE)) return list;
      } else {
        return list;
      }
    }
  }
  return NULL;
}


int vLuaPlugin::push(LuaState *lua, void *data) {
  if (data) {
    lua->PushLightUserData(data);
    return 1;
  }
  return Nil(lua);
}


int vLuaPlugin::push(LuaState *lua, int data) {
  lua->PushInteger(data);
  return 1;
}


int vLuaPlugin::push(LuaState *lua, bool data) {
  lua->PushBoolean(data);
  return 1;
}


int vLuaPlugin::push(LuaState *lua, const char *data) {
  if (data) {
    lua->PushString(data);
    return 1;
  }
  return Nil(lua);
}






//get the list of plugins
int vLuaPlugin::_getFirstPlugin(LuaState *lua) {
  return push( lua, plg->getFirstPlugin() );
}

int vLuaPlugin::_getLastPlugin(LuaState *lua) {
  return push( lua, plg->getLastPlugin() );
}

int vLuaPlugin::_getPrevPlugin(LuaState *lua) {
  return push( lua, plg->getPrevPlugin(getPlugin(lua)) );
}

int vLuaPlugin::_getNextPlugin(LuaState *lua) {
  return push( lua, plg->getNextPlugin(getPlugin(lua)) );
}


int vLuaPlugin::_getPlugin(LuaState *lua) {
  if (lua->GetTop() > 0  &&  lua->Stack(1).IsString()) {
    return push( lua, plg->getPlugin(lua->Stack(1).GetString()) );
  }
  return Nil(lua);
}


int vLuaPlugin::_getPluginByFilename(LuaState *lua) {
  if (lua->GetTop() > 0  &&  lua->Stack(1).IsString()) {
    return push( lua, plg->getPluginByFilename(lua->Stack(1).GetString()) );
  }
  return Nil(lua);
}


//get info about the plugin
int vLuaPlugin::_getPluginVersion(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  if (plugin) return push( lua, (int)plugin->getPluginVersion() );
  return Nil(lua);
}

int vLuaPlugin::_getPluginSdkVersion(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  if (plugin) return push( lua, (int)plugin->getSdkVersion() );
  return Nil(lua);
}

int vLuaPlugin::_getPluginLibVersion(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  if (plugin) return push( lua, (int)plugin->getLibVersion() );
  return Nil(lua);
}

int vLuaPlugin::_getPluginName(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  if (plugin) return push( lua, plugin->getName() );
  return Nil(lua);
}

int vLuaPlugin::_getPluginFilename(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  if (plugin) return push( lua, plugin->getFilename() );
  return Nil(lua);
}



//get lists from plugin

int vLuaPlugin::_root_getFirstChild(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  if (plugin) return push( lua, plugin->root_getFirstChild() );
  return Nil(lua);
}

int vLuaPlugin::_root_getLastChild(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  if (plugin) return push( lua, plugin->root_getLastChild() );
  return Nil(lua);
}

int vLuaPlugin::_root_getPrevChild(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->root_getPrevChild(list) );
  return Nil(lua);
}

int vLuaPlugin::_root_getNextChild(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->root_getNextChild(list) );
  return Nil(lua);
}


//some special stuff

int vLuaPlugin::_root_getList(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  if (plugin  &&  lua->GetTop() == 2  &&  lua->Stack(2).IsInteger()) {
    return push( lua, plugin->root_getList(lua->Stack(2).GetInteger()) );
  }

  const char *name   = NULL;
  const char *type   = NULL;
  if (lua->GetTop() > 1  &&  lua->Stack(2).IsString()) name   = lua->Stack(2).GetString();
  if (lua->GetTop() > 2  &&  lua->Stack(3).IsString()) type   = lua->Stack(3).GetString();

  //TODO:  build a xmlParamList here of everything else on the Lua stack

  if (plugin  &&  name) return push( lua, plugin->root_getList(name, type, NULL) );
  return Nil(lua);
}


int vLuaPlugin::_root_getRoot(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  if (plugin) return push( lua, plugin->list_getRoot() );
  return Nil(lua);
}

int vLuaPlugin::_root_getIdent(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  if (plugin) return push( lua, plugin->root_getIdent() );
  return Nil(lua);
}


int vLuaPlugin::_root_hasList(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  return push( lua, list!=NULL );
}


//primary list information
int vLuaPlugin::_list_getName(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_getName(list) );
  return Nil(lua);
}

int vLuaPlugin::_list_setName(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list = getList(plugin, lua);
  if (list) {
    if (lua->GetTop() > 2  &&  lua->Stack(3).IsString()){
      plugin->list_setName(list, lua->Stack(3).GetString());
      return push(lua, true);
    }
  }
  return Nil(lua);
}

int vLuaPlugin::_list_getType(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_getType(list) );
  return Nil(lua);
}



int vLuaPlugin::_list_message(LuaState *lua) {
  //TODO:  implement this, HAHAHAA!!!
  return 0;
}




//list user data
int vLuaPlugin::_list_getUserData(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_getUserData(list) );
  return Nil(lua);
}

int vLuaPlugin::_list_setUserData(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list = getList(plugin, lua);
  if (list) {
    if (lua->GetTop() > 2  &&  lua->Stack(3).IsString()){
      plugin->list_setUserData(list, lua->Stack(3).GetString());
      return push(lua, true);
    }
  }
  return Nil(lua);
}




//list flags
int vLuaPlugin::_list_getFlags(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, (int)plugin->list_getFlags(list) );
  return Nil(lua);
}


int vLuaPlugin::_list_getSecurity(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, (int)plugin->list_getSecurity(list) );
  return Nil(lua);
}


int vLuaPlugin::_list_getSecurityEx(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, (int)plugin->list_getSecurityEx(list) );
  return Nil(lua);
}



//list attributes
int vLuaPlugin::_list_isVisible(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_isVisible(list)==TRUE );
  return Nil(lua);
}

int vLuaPlugin::_list_setVisible(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) {
    if (lua->GetTop() > 2  &&  lua->Stack(3).IsBoolean()) {
      plugin->list_setVisible(list, lua->Stack(3).GetBoolean());
      return push(lua, true);
    }
  }
  return Nil(lua);
}

int vLuaPlugin::_list_hasChildrenVisible(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  BOOL     recurse = FALSE;
  if (lua->GetTop() > 2  &&  lua->Stack(3).IsBoolean()) recurse = lua->Stack(3).GetBoolean();
  if (list) return push( lua, plugin->list_hasChildrenVisible(list, recurse)==TRUE );
  return Nil(lua);
}



int vLuaPlugin::_list_isEnabled(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_isEnabled(list)==TRUE );
  return Nil(lua);
}

int vLuaPlugin::_list_setEnabled(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) {
    if (lua->GetTop() > 2  &&  lua->Stack(3).IsBoolean()) {
      plugin->list_setEnabled(list, lua->Stack(3).GetBoolean());
      return push(lua, true);
    }
  }
  return Nil(lua);
}

int vLuaPlugin::_list_hasChildrenEnabled(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  BOOL     recurse = FALSE;
  if (lua->GetTop() > 2  &&  lua->Stack(3).IsBoolean()) recurse = lua->Stack(3).GetBoolean();
  if (list) return push( lua, plugin->list_hasChildrenEnabled(list, recurse)==TRUE );
  return Nil(lua);
}



int vLuaPlugin::_list_isHighlighted(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_isHighlighted(list)==TRUE );
  return Nil(lua);
}

int vLuaPlugin::_list_setHighlighted(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) {
    if (lua->GetTop() > 2  &&  lua->Stack(3).IsBoolean()) {
      plugin->list_setHighlighted(list, lua->Stack(3).GetBoolean());
      return push(lua, true);
    }
  }
  return Nil(lua);
}

int vLuaPlugin::_list_hasChildrenHighlighted(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  BOOL     recurse = FALSE;
  if (lua->GetTop() > 2  &&  lua->Stack(3).IsBoolean()) recurse = lua->Stack(3).GetBoolean();
  if (list) return push( lua, plugin->list_hasChildrenHighlighted(list, recurse)==TRUE );
  return Nil(lua);
}



int vLuaPlugin::_list_isExpanded(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_isExpanded(list)==TRUE );
  return Nil(lua);
}

int vLuaPlugin::_list_setExpanded(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) {
    if (lua->GetTop() > 2  &&  lua->Stack(3).IsBoolean()) {
      plugin->list_setExpanded(list, lua->Stack(3).GetBoolean());
      return push(lua, true);
    }
  }
  return Nil(lua);
}

int vLuaPlugin::_list_hasChildrenExpanded(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  BOOL     recurse = FALSE;
  if (lua->GetTop() > 2  &&  lua->Stack(3).IsBoolean()) recurse = lua->Stack(3).GetBoolean();
  if (list) return push( lua, plugin->list_hasChildrenExpanded(list, recurse)==TRUE );
  return Nil(lua);
}



int vLuaPlugin::_list_isSavable(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_isSavable(list)==TRUE );
  return Nil(lua);
}

int vLuaPlugin::_list_setSavable(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) {
    if (lua->GetTop() > 2  &&  lua->Stack(3).IsBoolean()) {
      plugin->list_setSavable(list, lua->Stack(3).GetBoolean());
      return push(lua, true);
    }
  }
  return Nil(lua);
}

int vLuaPlugin::_list_hasChildrenSavable(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  BOOL     recurse = FALSE;
  if (lua->GetTop() > 2  &&  lua->Stack(3).IsBoolean()) recurse = lua->Stack(3).GetBoolean();
  if (list) return push( lua, plugin->list_hasChildrenSavable(list, recurse)==TRUE );
  return Nil(lua);
}







//get and set param value from a list
int vLuaPlugin::_list_getParam(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list = getList(plugin, lua);
  if (list) {
    if (lua->GetTop() > 2  &&  lua->Stack(3).IsString()){
      return push( lua, plugin->list_getParam(list, lua->Stack(3).GetString()) );
    }
  }
  return Nil(lua);
}


int vLuaPlugin::_list_setParam(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list = getList(plugin, lua);
  if (list) {
    if (lua->GetTop() > 3  &&  lua->Stack(3).IsString()) {
      if (lua->Stack(4).IsInteger ()) {
        plugin->list_setParamInt(list, lua->Stack(3).GetString(), lua->Stack(4).GetInteger());
      } else if (lua->Stack(4).IsNumber()) {
        plugin->list_setParamNumber(list, lua->Stack(3).GetString(), lua->Stack(4).GetNumber());
      } else if (lua->Stack(4).IsString()) {
        plugin->list_setParam(list, lua->Stack(3).GetString(), lua->Stack(4).GetString());
      } else {
        plugin->list_setParam(list, lua->Stack(3).GetString(), NULL);
      }
      return push(lua, true);
    }
  }
  return Nil(lua);
}


int vLuaPlugin::_list_hasParam(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list = getList(plugin, lua);
  if (list) {
    if (lua->GetTop() > 2  &&  lua->Stack(3).IsString()){
      return push( lua, plugin->list_hasParam(list, lua->Stack(3).GetString())==TRUE );
    }
  }
  return Nil(lua);
}

int vLuaPlugin::_list_hasParams(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list = getList(plugin, lua);
  if (list) return push( lua, plugin->list_hasParams(list)==TRUE );
  return Nil(lua);
}

int vLuaPlugin::_list_insertParam(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list = getList(plugin, lua);
  if (list) {
    if (lua->GetTop() > 3  &&  lua->Stack(3).IsString()) {
      if (lua->Stack(4).IsInteger ()) {
        plugin->list_insertParamInt(list, lua->Stack(3).GetString(), lua->Stack(4).GetInteger());
      } else if (lua->Stack(4).IsString()) {
        plugin->list_insertParam(list, lua->Stack(3).GetString(), lua->Stack(4).GetString());
      } else {
        plugin->list_insertParam(list, lua->Stack(3).GetString(), NULL);
      }
      return push(lua, true);
    }
  }
  return Nil(lua);
}

int vLuaPlugin::_list_insertParamNoSave(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list = getList(plugin, lua);
  if (list) {
    if (lua->GetTop() > 3  &&  lua->Stack(3).IsString()) {
      if (lua->Stack(4).IsInteger ()) {
        plugin->list_insertParamIntNoCfg(list, lua->Stack(3).GetString(), lua->Stack(4).GetInteger());
      } else if (lua->Stack(4).IsString()) {
        plugin->list_insertParamNoCfg(list, lua->Stack(3).GetString(), lua->Stack(4).GetString());
      } else {
        plugin->list_insertParamNoCfg(list, lua->Stack(3).GetString(), NULL);
      }
      lua->PushBoolean(true);
      return 1;
    }
  }
  lua->PushNil();
  return 1;
}


int vLuaPlugin::_list_getParamCount(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_getParamCount(list) );
  return Nil(lua);
}


int vLuaPlugin::_list_getParamFlags(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list = getList(plugin, lua);
  const char *param = NULL;
  if (lua->GetTop() > 3  &&  lua->Stack(3).IsString()) param = lua->Stack(3).GetString();
  if (list) return push( lua, (int)plugin->list_getParamFlags(list, param) );
  return Nil(lua);
}


int vLuaPlugin::_list_isParamSavable(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  const char *param = NULL;
  if (lua->GetTop() > 3  &&  lua->Stack(3).IsString()) param = lua->Stack(3).GetString();
  if (list) return push( lua, plugin->list_isParamSavable(list, param)==TRUE );
  return Nil(lua);
}


int vLuaPlugin::_list_removeParam(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  const char *param = NULL;
  if (lua->GetTop() > 3  &&  lua->Stack(3).IsString()) param = lua->Stack(3).GetString();
  if (list) {
    plugin->list_removeParam(list, param);
    return push(lua,  true);
  }
  return Nil(lua);
}



int vLuaPlugin::_list_getParent(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_getParent(list) );
  return Nil(lua);
}

int vLuaPlugin::_list_getChild(LuaState *lua) {
  vPlugin    *plugin = getPlugin(lua);
  HLIST       list   = getList(plugin, lua);
  const char *name   = NULL;
  const char *type   = NULL;
  if (lua->GetTop() > 2  &&  lua->Stack(3).IsString()) name   = lua->Stack(3).GetString();
  if (lua->GetTop() > 3  &&  lua->Stack(4).IsString()) type   = lua->Stack(4).GetString();

  //TODO:  build a xmlParamList here of everything else on the Lua stack

  if (plugin  &&  list  &&  name) return push( lua, plugin->list_getChild(list, name, type, NULL) );
  return Nil(lua);
}

//todo: fix this to support RMXLONG
int vLuaPlugin::_list_getChildById(LuaState *lua) {
  vPlugin    *plugin = getPlugin(lua);
  HLIST       list   = getList(plugin, lua);
  int         id     = 0;
  bool        recur  = false;
  if (lua->GetTop() > 2  &&  lua->Stack(3).IsInteger()) id    = lua->Stack(3).GetInteger();
  if (lua->GetTop() > 3  &&  lua->Stack(4).IsBoolean()) recur = lua->Stack(4).GetBoolean();
  if (plugin  &&  list  &&  id) return push( lua, plugin->list_getChildById(list, id, recur) );
  return push(lua, false);
}

int vLuaPlugin::_list_hasChild(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  HLIST    list2  = getList(plugin, lua, 3);
  BOOL recurse = false;
  if (lua->GetTop() > 3  &&  lua->Stack(4).IsBoolean()) recurse = lua->Stack(4).GetBoolean();
  if (list) return push( lua, plugin->list_hasChild(list, list2, recurse)==TRUE );
  return Nil(lua);
}

int vLuaPlugin::_list_hasChildren(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_hasChildren(list)==TRUE );
  return Nil(lua);
}

int vLuaPlugin::_list_getChildCount(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_getChildCount(list) );
  return Nil(lua);
}



int vLuaPlugin::_list_getFirstChild(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_getFirstChild(list) );
  lua->PushNil();
  return 1;
}

int vLuaPlugin::_list_getLastChild(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_getLastChild(list) );
  lua->PushNil();
  return 1;
}

int vLuaPlugin::_list_getPrevChild(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_getPrevChild( list, getList(plugin, lua, 3, FALSE) ) );
  lua->PushNil();
  return 1;
}

int vLuaPlugin::_list_getNextChild(LuaState *lua) {
  vPlugin *plugin = getPlugin(lua);
  HLIST    list   = getList(plugin, lua);
  if (list) return push( lua, plugin->list_getNextChild( list, getList(plugin, lua, 3, FALSE) ) );
  lua->PushNil();
  return 1;
}

#endif //_RMX_LUA_
