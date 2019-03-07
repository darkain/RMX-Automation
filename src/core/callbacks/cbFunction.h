/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __FUNCTIONCALLBACK_H__
#define __FUNCTIONCALLBACK_H__


#include "cbPlugin.h"
#include "../../../sdk/cb/cbRoot.h"
#include "../Lua/luaHelper.h"


#define _RMX_CORE_FUNCTION_		0x01
#define _RMX_CORE_GROUP_		0x02


class cbCoreFunction : public cbCore {
  public:
    cbCoreFunction(HPLUGIN cb_plugin, HLIST cb_list, int newtype, cbCoreFunction *par=NULL);
    virtual ~cbCoreFunction();


  protected:
//    virtual void cb_onInsertChild(HLIST child, HLIST insert);
		virtual void cb_cfgMessage(cbMessage message, HLIST child, HLIST insert=NULL);

#ifdef _RMX_LUA_
    virtual int onScriptActivated(LuaState *lua);
#endif //_RMX_LUA_

  
  public:
    inline const char *getFunctionName() const { return getName();          }
    inline const char *getScriptName()   const { return getParam("script"); }

    void activateFunction(HPLUGIN source, RMXLONG param1, RMXLONG param2);
    void setEventDescription(HPLUGIN source, RMXLONG param1, RMXLONG param2);
    static void setDefaultDescription(HPLUGIN source, RMXLONG param1, RMXLONG param2);

    static cbCoreFunction *getFirstFunction() { return rootfunctions.getFirstItem(); }
    static cbCoreFunction *getLastFunction( ) { return rootfunctions.getLastItem( ); }
    static cbCoreFunction *getPrevFunction(cbCoreFunction *item) { return rootfunctions.getPrevItem(item); }
    static cbCoreFunction *getNextFunction(cbCoreFunction *item) { return rootfunctions.getNextItem(item); }
    static cbCoreFunction *getFunction(RMXLONG pluginid, RMXLONG listid);
    static cbCoreFunction *getFunction(HPLUGIN plugin, HLIST list, int searchtype=0);
    static cbCoreFunction *getFunctionByHint(const char *hint, bool recursive=true);
    static cbCoreFunction *getFunctionByScriptName(const char *script, bool recursive=true);

    inline cbCoreFunction *getFirstChildFunction() { return children.getFirstItem(); }
    inline cbCoreFunction *getLastChildFunction( ) { return children.getLastItem( ); }
    inline cbCoreFunction *getPrevChildFunction(cbCoreFunction *item) { return children.getPrevItem(item); }
    inline cbCoreFunction *getNextChildFunction(cbCoreFunction *item) { return children.getNextItem(item); }

    cbCoreFunction *getChildFunction(HPLUGIN plugin, HLIST list);
    cbCoreFunction *getChildFunctionByHint(const char *hint, bool recursive=true);
    cbCoreFunction *getChildFunctionByScriptName(const char *script, bool recursive=true);

    inline int  getFunctionType() const { return listtype; }
	inline bool isFunction()      const { return listtype == _RMX_CORE_FUNCTION_; }
	inline bool isGroup()         const { return listtype == _RMX_CORE_GROUP_;    }

    inline cbCoreFunction *getFunctionParent() const { return parent; }


#ifdef _RMX_LUA_
    static void registerScripts();
           void registerScript();
    static void deregisterScripts();
           void deregisterScript();

    inline LuaObject getScriptObject() { return scriptobject; }
    inline BOOL hasScriptObject() const { return (scriptobject.GetState() != NULL); }

    BOOL script_isValueNull(   const char *name);
    BOOL script_isValueBoolean(const char *name);
    BOOL script_isValueInteger(const char *name);
    BOOL script_isValueNumber( const char *name);
    BOOL script_isValueString( const char *name);

    void script_setValueBoolean(const char *name, BOOL        value);
    void script_setValueInteger(const char *name, int         value);
    void script_setValueNumber( const char *name, double      value);
    void script_setValueString( const char *name, const char *value);

    BOOL        script_getValueBoolean(const char *name);
    int         script_getValueInteger(const char *name);
    double      script_getValueNumber( const char *name);
    const char *script_getValueString( const char *name);


    static BOOL global_isValueNull(   const char *name);
    static BOOL global_isValueBoolean(const char *name);
    static BOOL global_isValueInteger(const char *name);
    static BOOL global_isValueNumber( const char *name);
    static BOOL global_isValueString( const char *name);

    static void global_setValueBoolean(const char *name, BOOL        value);
    static void global_setValueInteger(const char *name, int         value);
    static void global_setValueNumber( const char *name, double      value);
    static void global_setValueString( const char *name, const char *value);

    static BOOL        global_getValueBoolean(const char *name);
    static int         global_getValueInteger(const char *name);
    static double      global_getValueNumber( const char *name);
    static const char *global_getValueString( const char *name);
#endif //_RMX_LUA_

    static int getQueDepth() { return que_depth; }


private:
    int listtype;
    cbCoreFunction *parent;
    rmxList<cbCoreFunction*> children;

    static rmxList<cbCoreFunction*> rootfunctions;
    static int que_depth;

#ifdef _RMX_LUA_
    LuaObject scriptobject;
#endif //_RMX_LUA_
};


//-----------------------------------------------------------------------------------------------------------------------------------------


class cbRootFunction : public cbRoot {
  public:
    cbRootFunction(HPLUGIN plugin) : cbRoot(plugin) {
      HLIST list = plugin->root_getFirstChild();
      while (list) {
        const char *type = plugin->list_getType(list);
        if (type == NULL) {}
        else if (VSTRCMP(type, "function")==0) new cbCoreFunction(getRemotePlugin(), list, _RMX_CORE_FUNCTION_);
        else if (VSTRCMP(type, "group")   ==0) new cbCoreFunction(getRemotePlugin(), list, _RMX_CORE_GROUP_);
        list = plugin->root_getNextChild(list);
      }
    }

    virtual ~cbRootFunction() {
#ifdef _RMX_DEBUG_
      char str[256]="";
      VPRINTF(str, sizeof(str), "%p", (cbCore*)this);
      CONSOLE_MAIN->printDebug("FUNC", str);
#endif //_RMX_DEBUG_
    }

  protected:
    virtual void cb_onInsertChild(HLIST child, HLIST insert) {
      const char *type = getRemotePlugin()->list_getType(child);
      if (type == NULL) {}
      else if (type && VSTRCMP(type, "function") == 0) new cbCoreFunction(getRemotePlugin(), child, _RMX_CORE_FUNCTION_);
      else if (type && VSTRCMP(type, "group")    == 0) new cbCoreFunction(getRemotePlugin(), child, _RMX_CORE_GROUP_);
    }

		virtual void cb_cfgMessage(cbMessage message, HLIST  child, HLIST  insert=NULL) {
			if (message == CB_CHILD_INSERT) {
				const char *type = getRemotePlugin()->list_getType(child);
				if (type == NULL) {}
				else if (type && VSTRCMP(type, "function") == 0) new cbCoreFunction(getRemotePlugin(), child, _RMX_CORE_FUNCTION_);
				else if (type && VSTRCMP(type, "group")    == 0) new cbCoreFunction(getRemotePlugin(), child, _RMX_CORE_GROUP_);
			}
		}

};


//-----------------------------------------------------------------------------------------------------------------------------------------


class vFunctionPluginCallback : public vPluginCallback {
  public:
    vFunctionPluginCallback() {
      vPlugin *plugin = plg->getFirstPlugin();
      while (plugin) {
#ifdef RMX_STANDALONE
        new cbRootFunction((HPLUGIN)plugin);
#else
        if (!VSTRCMP(plugin->getPathRootName(), "Embedded")) new cbRootFunction((HPLUGIN)plugin);
#endif
        plugin = plg->getNextPlugin(plugin);
      }
    }


    virtual ~vFunctionPluginCallback() {
    }


  protected:
#ifdef RMX_STANDALONE
    virtual void onAppendPlugin(vPlugin *plugin) { new cbRootFunction((HPLUGIN)plugin); }
#else
    virtual void onAppendPlugin(vPlugin *plugin) {
      if (VSTRCMP(plugin->getPathRootName(), "Embedded")==0) new cbRootFunction((HPLUGIN)plugin);
    }
#endif
};


#endif //__FUNCTIONCALLBACK_H__
