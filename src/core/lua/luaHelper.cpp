/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007, 2009 Vincent E. Milum Jr., All rights reserved.  *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "luaHelper.h"
#ifdef _RMX_LUA_


#include "../Loader.h"
#include "../callbacks/cbFunction.h"

#include "luaTimer.h"
#include "luaConsole.h"
#include "luaPlugin.h"

#ifdef LUA_COMOBJECT
#include <luacom.h>
#include "objbase.h"
#endif //LUA_COMOBJECT


extern "C" {
  #include "../../../libraries/luaplus/Src/LuaPlus/include/lualib.h"
}


using namespace LuaPlus;
using namespace LPCD;


LuaState *lua_handle = NULL;
int lua_err = 0;


void lua_create() {
#ifdef LUA_COMOBJECT
  OleInitialize(NULL);
#endif //LUA_COMOBJECT

  lua_handle = LuaState::Create(false, false);
  lua_err = lua_openLibs(lua_handle);

#ifdef LUA_COMOBJECT
  luacom_open(lua_handle->GetCState());
#endif //LUA_COMOBJECT
}


void lua_destroy() {
  CONSOLE_MAIN->print("Lua", "Stopping");
  vLuaTimer::stopAllTimers();

#ifdef LUA_COMOBJECT
  luacom_close(lua_handle->GetCState());
#endif //LUA_COMOBJECT

  cbCoreFunction::deregisterScripts();

  LuaState::Destroy(lua_handle);
  lua_handle = NULL;
}


void lua_start() {
  CONSOLE_MAIN->print("Lua", "Starting");

  cbCoreFunction::registerScripts();

  lua_handle->GetGlobals().SetNumber("NULL", 0);
  lua_handle->GetGlobals().SetNumber("null", 0);

  int len = MAX_PATH * 2 + 10;
  char *path = (char*)malloc(len);
  VPRINTF(path, len, "?;?.lua;%s\\rmx_scripts\\?;%s\\rmx_scripts\\?.lua", RMXOS::getPath(), RMXOS::getPath());
  lua_handle->GetGlobals().SetString("LUA_PATH", path);

  VPRINTF(path, len, "%s\\rmx_scripts\\main.lua", RMXOS::getPath());
  if (VFEXISTS(path)) {
    lua_handle->DoFile(path);
  }
  free(path);
}


void lua_script(const char *text) {
  if (!text  ||  !*text) return;

  int len = (int)VSTRLEN(text);
  char *command = (char*)malloc(len+3);
  memcpy(command, text, len+1);

  //remove white-space at begining of command
  char *bgn = command;  
  while ( (*bgn == ' ') || (*bgn == '\r') || (*bgn == '\n') || (*bgn == '\t') ) {
    bgn++;
  }

  //remove white-space at end of command
  char *pstr = bgn;
  while (*pstr != NULL) pstr++;
  pstr--;
  while ( pstr > bgn  &&  ((*pstr == ' ') || (*pstr == '\r') || (*pstr == '\n') || (*pstr == '\t')) ) {
    *pstr = NULL;
    pstr--;
  }


  //check to see if command needs a () at the end of it
  pstr = bgn;
  bool ok = true;
  while (*pstr) {
    if ( (*pstr == ' ') || (*pstr == '=') || (*pstr == '\'')
      || (*pstr == '\"') || (*pstr == '(') || (*pstr == ')') ) {

      ok = false;
      break;
    }
    pstr++;
  }

  if (ok) {
    *pstr = '(';
    pstr++;
    *pstr = ')';
    pstr++;
    *pstr = NULL;
  }

  if (*bgn) {
    lua_handle->DoBuffer(bgn, VSTRLEN(bgn), "SCRIPT");
  }

  free(command);
  return;
}



LuaState *getLuaHandle() {
  return lua_handle;
}


// a list of libs to expose for use in scripts
static const luaL_reg lualibs[] = {
  {"base",    luaopen_base},
  {"table",   luaopen_table},
  {"string",  luaopen_string},
  {"wstring", luaopen_wstring},
  {"math",    luaopen_math},

#if defined LUA_COMOBJECT | defined RMX_STANDALONE
  {"io",      luaopen_io},
#endif

#ifdef RMX_STANDALONE
  {"debug",   luaopen_debug},
  {"loadlib", luaopen_loadlib},   //gotta see what functions this has, and override them, maybe?
#endif
  {NULL, NULL}
};


int lua_dostring(LuaState *lua) {
  if (lua->GetTop()>0 && lua->Stack(1).IsString()) lua->DoString(lua->Stack(1).GetString());
  return 0;
}


int lua_toHex(LuaState *lua) {
  int top = lua->GetTop();
  for (int i=1; i<=top; i++) {
    char str[16] = "0x00000000";
    if (lua->Stack(i).IsInteger()) {
      VPRINTF(str, sizeof(str), "0x%08X", lua->Stack(i).GetInteger());
    }
    lua->PushString(str);
  }
  return top;
}


int lua_fromHex(LuaState *lua) {
  int top = lua->GetTop();
  for (int i=1; i<=top; i++) {
    int val = 0;
    if (lua->Stack(i).IsString()) {
      if (VSCANF(lua->Stack(i).GetString(), "0x%08X", &val) != 1) val = 0;
    }
    lua->PushInteger(val);
  }
  return top;
}


int lua_color(LuaState *lua) {
  int top = lua->GetTop();
  for (int i=1; i<=top; i++) {
    if (lua->Stack(i).IsInteger()) {
      switch (lua->Stack(i).GetInteger()) {
        case 0:  lua->PushString("\033[0;30m");  break;
        case 1:  lua->PushString("\033[0;31m");  break;
        case 2:  lua->PushString("\033[0;32m");  break;
        case 3:  lua->PushString("\033[0;33m");  break;
        case 4:  lua->PushString("\033[0;34m");  break;
        case 5:  lua->PushString("\033[0;35m");  break;
        case 6:  lua->PushString("\033[0;36m");  break;
        case 7:  lua->PushString("\033[0;37m");  break;
        case 9:  lua->PushString("\033[0;39m");  break;

        case 10: lua->PushString("\033[1;30m");  break;
        case 11: lua->PushString("\033[1;31m");  break;
        case 12: lua->PushString("\033[1;32m");  break;
        case 13: lua->PushString("\033[1;33m");  break;
        case 14: lua->PushString("\033[1;34m");  break;
        case 15: lua->PushString("\033[1;35m");  break;
        case 16: lua->PushString("\033[1;36m");  break;
        case 17: lua->PushString("\033[1;37m");  break;
        case 19: lua->PushString("\033[1;39m");  break;
        default: lua->PushNil();  break;
      }
    } else if (lua->Stack(i).IsString()) {
      //check for colour names here
      lua->PushNil();
    } else {
      lua->PushNil();
    }
  }
  return top;
}



// load the list of libs
int lua_openLibs(LuaState *lua) {
  const luaL_reg *lib = lualibs;
  for (; lib->func; lib++) {
    lib->func(lua->GetCState());  
    lua_settop(lua->GetCState(), 0);
  }

  LuaObject obj = lua->GetGlobals();
  vLuaConsole console(obj);

//  obj.Register("rmx",         lua_Rmx);  //"rmx" is going to become a table for general purpose data
//  obj.Register("messageBox",  lua_MessageBox);
//  obj.Register("MsgBox",      lua_MessageBox);
  obj.Register("dostring",    lua_dostring);
  obj.Register("eval",        lua_dostring);
  obj.Register("toHex",       lua_toHex);
  obj.Register("fromHex",     lua_fromHex);
  obj.Register("color",       lua_color);

//  obj.Register("include",     lua_Include);

  vLuaTimer::init(obj);
  vLuaPlugin::init(obj);
  return obj.GetCount();
}


#endif//_RMX_LUA_
