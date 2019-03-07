/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __LUAPLUGIN_H__
#define __LUAPLUGIN_H__


#include "luaHelper.h"
#ifdef _RMX_LUA_


class vPlugin;


class vLuaPlugin {
  public:
    static void init(LuaObject globals);

    static vPlugin *getPlugin(LuaState *lua);
    static HLIST getList(vPlugin *plugin, LuaState *lua, int pos=2, BOOL verify=TRUE);

    static int push(LuaState *lua, void *data);
    static int push(LuaState *lua, int data);
    static int push(LuaState *lua, bool data);
    static int push(LuaState *lua, const char *data);

    inline static int Nil(LuaState *lua) { lua->PushNil();  return 1; }

    //get the list of plugins
    static int _getFirstPlugin(LuaState *lua);
    static int _getLastPlugin(LuaState *lua);
    static int _getPrevPlugin(LuaState *lua);
    static int _getNextPlugin(LuaState *lua);
    static int _getPlugin(LuaState *lua);
    static int _getPluginByFilename(LuaState *lua);

    //get info about the plugin
    static int _getPluginVersion(LuaState *lua);
    static int _getPluginSdkVersion(LuaState *lua);
    static int _getPluginLibVersion(LuaState *lua);
    static int _getPluginName(LuaState *lua);
    static int _getPluginFilename(LuaState *lua);


    //vConfigList/HLIST based stuff - standardized to the vPlugin* API

    static int _root_getFirstChild(LuaState *lua);
    static int _root_getLastChild(LuaState *lua);
    static int _root_getPrevChild(LuaState *lua);
    static int _root_getNextChild(LuaState *lua);
    static int _root_getList(LuaState *lua);
    static int _root_getRoot(LuaState *lua);
    static int _root_getIdent(LuaState *lua);
    static int _root_hasList(LuaState *lua);


    static int _list_getName(LuaState *lua);
    static int _list_setName(LuaState *lua);
    static int _list_getType(LuaState *lua);
    static int _list_getUnique(LuaState *lua);
    static int _list_message(LuaState *lua);

    static int _list_getUserData(LuaState *lua);
    static int _list_setUserData(LuaState *lua);

    static int _list_getFlags(LuaState *lua);
    static int _list_getSecurity(LuaState *lua);
    static int _list_getSecurityEx(LuaState *lua);

    static int _list_isVisible(LuaState *lua);
    static int _list_setVisible(LuaState *lua);
    static int _list_hasChildrenVisible(LuaState *lua);

    static int _list_isEnabled(LuaState *lua);
    static int _list_setEnabled(LuaState *lua);
    static int _list_hasChildrenEnabled(LuaState *lua);

    static int _list_isHighlighted(LuaState *lua);
    static int _list_setHighlighted(LuaState *lua);
    static int _list_hasChildrenHighlighted(LuaState *lua);

    static int _list_isExpanded(LuaState *lua);
    static int _list_setExpanded(LuaState *lua);
    static int _list_hasChildrenExpanded(LuaState *lua);

    static int _list_isSavable(LuaState *lua);
    static int _list_setSavable(LuaState *lua);
    static int _list_hasChildrenSavable(LuaState *lua);

 
    static int _list_getParam(LuaState *lua);
    static int _list_setParam(LuaState *lua);
    static int _list_hasParam(LuaState *lua);
    static int _list_hasParams(LuaState *lua);

    static int _list_insertParam(LuaState *lua);
    static int _list_insertParamNoSave(LuaState *lua);
    static int _list_getParamCount(LuaState *lua);
    static int _list_getParamFlags(LuaState *lua);
    static int _list_isParamSavable(LuaState *lua);
    static int _list_removeParam(LuaState *lua);


    static int _list_getParent(LuaState *lua);
    static int _list_getChild(LuaState *lua);
    static int _list_getChildById(LuaState *lua);
    static int _list_hasChild(LuaState *lua);
    static int _list_hasChildren(LuaState *lua);
    static int _list_getChildCount(LuaState *lua);

    static int _list_getFirstChild(LuaState *lua);
    static int _list_getLastChild(LuaState *lua);
    static int _list_getPrevChild(LuaState *lua);
    static int _list_getNextChild(LuaState *lua);
};


#endif //_RMX_LUA_


#endif //__LUAPLUGIN_H__
