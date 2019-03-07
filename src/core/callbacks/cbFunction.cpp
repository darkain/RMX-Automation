/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"
#include "cbFunction.h"
#include "cbConsole.h"
#include "../Loader.h"
#include "../LocalPlugin.h"


rmxList<cbCoreFunction*> cbCoreFunction::rootfunctions;
int cbCoreFunction::que_depth = 0;


cbCoreFunction::cbCoreFunction(HPLUGIN cb_plugin, HLIST cb_list, int newtype, cbCoreFunction *par)
 : cbCore(cb_plugin, cb_list) {

  listtype = newtype;
  parent = par;

  if (parent) {
    parent->children.appendItem(this);
  } else {
    rootfunctions.appendItem(this);
  }

#ifdef _RMX_LUA_
  registerScript();
#endif //_RMX_LUA_


  HLIST child = cb_plugin->list_getFirstChild(cb_list);
  while (child) {
    const char *type = cb_plugin->list_getType(child);
    if (type == NULL) {}
    else if (VSTRCMP(type, "function") == 0) { new cbCoreFunction(getRemotePlugin(), child, _RMX_CORE_FUNCTION_, this); }
    else if (VSTRCMP(type, "group")    == 0) { new cbCoreFunction(getRemotePlugin(), child, _RMX_CORE_GROUP_   , this); }
    child = cb_plugin->list_getNextChild(cb_list, child);
  }
}


cbCoreFunction::~cbCoreFunction() {
#ifdef _RMX_DEBUG_
  char str[256]="";
  VPRINTF(str, sizeof(str), "%p", (cbCore*)this);
  CONSOLE_MAIN->printDebug("FUNC", str);
#endif //_RMX_DEBUG_
      
  cbCoreFunction *child = children.getFirstItem();
  while (child) {
    delete child;
    children.removeItem(child);
    child = children.getFirstItem();
  }

#ifdef _RMX_LUA_
  deregisterScript();
#endif //_RMX_LUA_

  if (parent) {
    parent->children.removeItem(this);
  } else {
    rootfunctions.removeItem(this);
  }
}


//void cbCoreFunction::cb_onInsertChild(HLIST child, HLIST insert) {
void cbCoreFunction::cb_cfgMessage(cbMessage message, HLIST child, HLIST insert) {
	if (message == CB_CHILD_INSERT) {
		const char *type = getRemotePlugin()->list_getType(child);
		if (type == NULL) {}
		else if (VSTRCMP(type, "function") == 0) { new cbCoreFunction(getRemotePlugin(), child, _RMX_CORE_FUNCTION_, this); }
		else if (VSTRCMP(type, "group")    == 0) { new cbCoreFunction(getRemotePlugin(), child, _RMX_CORE_GROUP_,    this); }
	}

	cbCore::cb_cfgMessage(message, child, insert);
//  cbCore::cb_onInsertChild(child, insert);
}



#ifdef _RMX_LUA_
int cbCoreFunction::onScriptActivated(LuaState *lua) {
  cfgBase *root    = plg->getMasterPlugin();
  cfgBase *vstack  = new cfgBase("stack",  "stack",  root);
  cfgBase *vreturn = new cfgBase("return", "return", root);
  int top = lua->GetTop();
  vstack->setValueInt(top);

  HLIST child = getFirstChild();
  while (child) {
    vstack->insertParam(getRemotePlugin()->list_getParam(child, "paramname"), NULLSTR);
    child = getNextChild(child);
  }

  prmBase *param = vstack->getFirstParam();
  for (int i=1; i<=top; i++) {
    if (!param) {
      char str[20];
      VPRINTF(str, sizeof(str), "param%d", i);
      param = vstack->insertParam(str, NULLSTR);
    }

    LuaStackObject object = lua->Stack(i);
    if      (object.IsNil())     param->setValueNull();
    else if (object.IsBoolean()) param->setValueInt(           object.GetBoolean());
    else if (object.IsInteger()) param->setValueInt(           object.GetInteger());
    else if (object.IsNumber())  param->setValueNumber(        object.GetNumber() );
    else if (object.IsWString()) param->setValueWide((wchar_t*)object.GetWString());
    else if (object.IsString())  param->setValueString(        object.GetString() );  //set value string, no conversions
    else                         param->setValueNull();
    param = vstack->getNextParam(param);
  }


  //activate function
  vstack->insertParam("style", "press");
  vstack->insertParamInt("value", 100);
  activateFunction(plg->getMasterHandle(), (RMXLONG)vstack, (RMXLONG)vreturn);


  //deactivate function
  vstack->setParamInt("value", 0);
  activateFunction(plg->getMasterHandle(), (RMXLONG)vstack, (RMXLONG)vreturn);


  int count = 0;
  param = vreturn->getFirstParam();
  while (param) {
    count++;
    if      (param->isNull())    lua->PushNil();
    else if (param->isInteger()) lua->PushInteger(param->getValueInt());
    else if (param->isNumber())  lua->PushNumber( param->getValueNumber());
    else if (param->isString())  lua->PushString( param->getValue());
    else                         lua->PushString( param->getValue());  //all other cases, use internal conversion
    param = vreturn->getNextParam(param);
  }

  delete vstack;
  delete vreturn;

  return count;
}
#endif //_RMX_LUA_


void cbCoreFunction::activateFunction(HPLUGIN source, RMXLONG param1, RMXLONG param2) {
  que_depth++;
  if (que_depth > 25) {
    CONSOLE_ACTIVE->printError("Core", "Infinite recursive loop detected");
  } else {
//    message(source, FUNCTION_ACTIVATE, param1, param2);

    //this is a cheap hack, since the API doesnt allow for "source" spoofing
    vPlugin *plugin = plg->getPlugin( getRemotePlugin() );
    plugin->list_message(getRemoteConfig(), source, FUNCTION_ACTIVATE, param1, param2);
  }
  que_depth--;
}


void cbCoreFunction::setEventDescription(HPLUGIN source, RMXLONG param1, RMXLONG param2) {
  HLIST list = (HLIST) param1;
  vPlugin *plugin = plg->getPlugin(source);
  if (!plugin) return;

  int len = 1;
  rmxList<const char*> names;
  cbCoreFunction *cb = this;
  while (cb) {
    const char *name = cb->getFunctionName();
    if (name) {
      len += ((int)VSTRLEN(name) + 3);
      names.appendItem(name);
    }
    cb = cb->getFunctionParent();
  }

  char *desc = (char*) malloc(len);
  *desc = NULL;

  const char *name = names.getLastItem();
  while (name) {
    VSTRCAT(desc, name, len);
    name = names.getPrevItem(name);
    if (name) VSTRCAT(desc, " > ", len);
  }

  plugin->list_setParam(list, "desc", desc);
  free(desc);
}


void cbCoreFunction::setDefaultDescription(HPLUGIN source, RMXLONG param1, RMXLONG param2) {
  HLIST list = (HLIST) param1;
  vPlugin *plugin = plg->getPlugin(source);
  if (!plugin) return;

  HLIST dest = (HLIST) plugin->list_getParamLong(list, "list");

  if (dest) {
    plugin->list_setParam(list, "desc", "[UNREGISTERED]");

  } else {
    const char *hint = plugin->list_getParamSafe(list, "hint");

    if (*hint) {
      char str[1024] = "[DEFAULT]";
      VPRINTF(str, sizeof(str), "[DEFAULT] (%s)", hint);
      plugin->list_setParam(list, "desc", str);

    } else {
      plugin->list_setParam(list, "desc", "[DEFAULT]");
    }
  }
}


cbCoreFunction *cbCoreFunction::getFunction(RMXLONG pluginid, RMXLONG listid) {
  vPlugin *plugin = plg->getPluginById(pluginid);
  if (!plugin) return NULL;

  HLIST list = plugin->root_getList(listid);
  if (!list) return NULL;

  cbCoreFunction *child = rootfunctions.getFirstItem();
  while (child) {
    cbCoreFunction *cb = child->getChildFunction((HPLUGIN)plugin, list);
    if (cb) return cb;
    child = rootfunctions.getNextItem(child);
  }
  return NULL;
}


cbCoreFunction *cbCoreFunction::getFunction(HPLUGIN plugin, HLIST list, int searchtype) {
  if (searchtype == 1) {
    cbCoreFunction *cb = rootfunctions.getFirstItem();
    while (cb) {
      cbCoreFunction *item = cb->getChildFunction((HPLUGIN)plugin, list);
      if (item) return item;
      cb = rootfunctions.getNextItem(cb);
    }
    return NULL;
  }

  RMXLONG pluginid = plugin->list_getParamLong(list, "plugin");
  RMXLONG listid   = plugin->list_getParamLong(list, "list");

  if (searchtype == 2  &&  pluginid == 0  &&  listid == 0) {
    return getFunctionByHint(plugin->list_getParam(list, "hint"));
  }

  return getFunction(pluginid, listid);
}


cbCoreFunction *cbCoreFunction::getFunctionByHint(const char *hint, bool recursive) {
  if (!hint)  return NULL;
  if (!*hint) return NULL;

  cbCoreFunction *cb = rootfunctions.getFirstItem();
  while (cb) {
    cbCoreFunction *item = cb->getChildFunctionByHint(hint, recursive);
    if (item) return item;
    cb = rootfunctions.getNextItem(cb);
  }

  return NULL;
}


cbCoreFunction *cbCoreFunction::getFunctionByScriptName(const char *script, bool recursive) {
  if (!script)  return NULL;
  if (!*script) return NULL;

  cbCoreFunction *cb = rootfunctions.getFirstItem();
  while (cb) {
    cbCoreFunction *item = cb->getChildFunctionByScriptName(script, recursive);
    if (item) return item;
    cb = rootfunctions.getNextItem(cb);
  }

  return NULL;
}



#ifdef _RMX_LUA_

void cbCoreFunction::registerScripts() {
  cbCoreFunction *cb = rootfunctions.getFirstItem();
  while (cb) {
    cb->registerScript();
    cb = rootfunctions.getNextItem(cb);
  }
}


void cbCoreFunction::registerScript() {
  LuaObject scriptparent;
  if (parent) {
    if (!parent->hasScriptObject()) return;
    scriptparent = parent->getScriptObject();
  } else {
    scriptparent = getLuaHandle()->GetGlobals();
  }

  const char *name = getScriptName();
  if (name  &&  *name) {
    if (listtype == 1) {
      scriptparent.Register(name, *this, &cbCoreFunction::onScriptActivated);
      scriptobject = scriptparent.GetByName(name);
    }
    else if (listtype == 2) {
      scriptobject = scriptparent.CreateTable(name);
    }
  }

  message(FUNCTION_START);

  cbCoreFunction *cb = children.getFirstItem();
  while (cb) {
    cb->registerScript();
    cb = children.getNextItem(cb);
  }
}


void cbCoreFunction::deregisterScripts() {
  cbCoreFunction *cb = rootfunctions.getFirstItem();
  while (cb) {
    cb->deregisterScript();
    cb = rootfunctions.getNextItem(cb);
  }
}


void cbCoreFunction::deregisterScript() {
  message(FUNCTION_STOP);

  cbCoreFunction *cb = children.getFirstItem();
  while (cb) {
    cb->deregisterScript();
    cb = children.getNextItem(cb);
  }

  scriptobject.AssignNil(getLuaHandle());
  scriptobject.Reset();
}


BOOL cbCoreFunction::script_isValueNull(const char *name) {
  return scriptobject.GetByName(name).IsNil();
}

BOOL cbCoreFunction::script_isValueBoolean(const char *name) {
  return scriptobject.GetByName(name).IsBoolean();
}

BOOL cbCoreFunction::script_isValueInteger(const char *name) {
  if (scriptobject.GetByName(name).IsInteger()) return TRUE;
  return scriptobject.GetByName(name).IsConvertibleToInteger();
}

BOOL cbCoreFunction::script_isValueNumber(const char *name) {
  if (scriptobject.GetByName(name).IsNumber()) return TRUE;
  return scriptobject.GetByName(name).IsConvertibleToNumber();
}

BOOL cbCoreFunction::script_isValueString(const char *name) {
  if (scriptobject.GetByName(name).IsString()) return TRUE;
  return scriptobject.GetByName(name).IsConvertibleToString();
}


void cbCoreFunction::script_setValueBoolean(const char *name, BOOL value) {
  scriptobject.SetBoolean(name, value!=0);
}

void cbCoreFunction::script_setValueInteger(const char *name, int value) {
  scriptobject.SetInteger(name, value);
}

void cbCoreFunction::script_setValueNumber(const char *name, double value) {
  scriptobject.SetNumber(name, value);
}

void cbCoreFunction::script_setValueString(const char *name, const char *value) {
  if (value) {
    scriptobject.SetString(name, value);
  } else {
    scriptobject.SetNil(name);
  }
}


BOOL cbCoreFunction::script_getValueBoolean(const char *name) {
  LuaObject object = scriptobject.GetByName(name);
  if (object.IsBoolean()) return object.GetBoolean();
  return FALSE;
}

int cbCoreFunction::script_getValueInteger(const char *name) {
  LuaObject object = scriptobject.GetByName(name);
  if (object.IsInteger()) return object.GetInteger();
  if (object.IsConvertibleToInteger()) return object.ToInteger();
  return 0;
}

double cbCoreFunction::script_getValueNumber(const char *name) {
  LuaObject object = scriptobject.GetByName(name);
  if (object.IsNumber()) return object.GetNumber();
  if (object.IsConvertibleToNumber()) return object.ToNumber();
  return 0;
}

const char *cbCoreFunction::script_getValueString(const char *name) {
  LuaObject object = scriptobject.GetByName(name);
  if (object.IsString()) return object.GetString();
  if (object.IsConvertibleToString()) return object.ToString();
  return NULL;
}



BOOL cbCoreFunction::global_isValueNull(const char *name) {
  LuaObject object = getLuaHandle()->GetGlobal(name);
  return object.IsNil();
}

BOOL cbCoreFunction::global_isValueBoolean(const char *name) {
  LuaObject object = getLuaHandle()->GetGlobal(name);
  return object.IsBoolean();
}

BOOL cbCoreFunction::global_isValueInteger(const char *name) {
  LuaObject object = getLuaHandle()->GetGlobal(name);
  if (object.IsInteger()) return true;
  return object.IsConvertibleToInteger();
}

BOOL cbCoreFunction::global_isValueNumber(const char *name) {
  LuaObject object = getLuaHandle()->GetGlobal(name);
  if (object.IsNumber()) return true;
  return object.IsConvertibleToNumber();
}

BOOL cbCoreFunction::global_isValueString(const char *name) {
  LuaObject object = getLuaHandle()->GetGlobal(name);
  if (object.IsString()) return true;
  return object.IsConvertibleToString();
}


void cbCoreFunction::global_setValueBoolean(const char *name, BOOL value) {
  getLuaHandle()->GetGlobals().SetBoolean(name, value!=0);
}

void cbCoreFunction::global_setValueInteger(const char *name, int value) {
  getLuaHandle()->GetGlobals().SetInteger(name, value);
}

void cbCoreFunction::global_setValueNumber(const char *name, double value) {
  getLuaHandle()->GetGlobals().SetNumber(name, value);
}

void cbCoreFunction::global_setValueString(const char *name, const char *value) {
  if (value) {
    getLuaHandle()->GetGlobals().SetString(name, value);
  } else {
    getLuaHandle()->GetGlobals().SetNil(name);
  }
}


BOOL cbCoreFunction::global_getValueBoolean(const char *name) {
  LuaObject object = getLuaHandle()->GetGlobal(name);
  if (object.IsBoolean()) return object.GetBoolean();
  return FALSE;
}

int cbCoreFunction::global_getValueInteger(const char *name) {
  LuaObject object = getLuaHandle()->GetGlobal(name);
  if (object.IsInteger()) return object.GetInteger();
  if (object.IsConvertibleToInteger()) return object.ToInteger();
  return 0;
}

double cbCoreFunction::global_getValueNumber(const char *name) {
  LuaObject object = getLuaHandle()->GetGlobal(name);
  if (object.IsNumber()) return object.GetNumber();
  if (object.IsConvertibleToNumber()) return object.ToNumber();
  return 0;
}

const char *cbCoreFunction::global_getValueString(const char *name) {
  LuaObject object = getLuaHandle()->GetGlobal(name);
  if (object.IsString()) return object.GetString();
  if (object.IsConvertibleToString()) return object.ToString();
  return NULL;
}

#endif //_RMX_LUA_



cbCoreFunction *cbCoreFunction::getChildFunction(HPLUGIN plugin, HLIST list) {
  if (plugin == getRemotePlugin()  &&  list == getRemoteConfig()) return this;

  cbCoreFunction *child = children.getFirstItem();
  while (child) {
    cbCoreFunction *cb = child->getChildFunction(plugin, list);
    if (cb) return cb;
    child = children.getNextItem(child);
  }

  return NULL;
}


cbCoreFunction *cbCoreFunction::getChildFunctionByHint(const char *hint, bool recursive) {
  if (!isEnabled()) return NULL;
  if (!VSTRCMP(hint, getParamSafe("hint"))) return this;
  if (!recursive) return NULL;

  cbCoreFunction *child = children.getFirstItem();
  while (child) {
    cbCoreFunction *cb = child->getChildFunctionByHint(hint);
    if (cb) return cb;
    child = children.getNextItem(child);
  }

  return NULL;
}


cbCoreFunction *cbCoreFunction::getChildFunctionByScriptName(const char *script, bool recursive) {
  if (!isEnabled()) return NULL;

  const char *name = getScriptName();
  if (name  &&  !VSTRCMP(script, name)) return this;

  if (!recursive) return NULL;

  cbCoreFunction *child = children.getFirstItem();
  while (child) {
    cbCoreFunction *cb = child->getChildFunctionByScriptName(script);
    if (cb) return cb;
    child = children.getNextItem(child);
  }

  return NULL;
}
