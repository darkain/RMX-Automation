/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "luaHelper.h"
#ifdef _RMX_LUA_


#include "../Loader.h"
#include "luaConsole.h"


vLuaConsole::vLuaConsole(LuaObject globals) {
  globals.Register("print",       message);
  globals.Register("echo",        message);
  globals.Register("printmain",   messageMain);
  globals.Register("echomain",    messageMain);
  globals.Register("printall",    messageAll);
  globals.Register("echoall",     messageAll);
  
  globals.Register("printsp",     messageSpace);
  globals.Register("echosp",      messageSpace);
  globals.Register("printspmain", messageSpaceMain);
  globals.Register("echospmain",  messageSpaceMain);
  globals.Register("printspall",  messageSpaceAll);
  globals.Register("echospall",   messageSpaceAll);
  
  globals.Register("printlf",     messageLine);
  globals.Register("echolf",      messageLine);
  globals.Register("printlfmain", messageLineMain);
  globals.Register("echolfmain",  messageLineMain);
  globals.Register("printlfall",  messageLineAll);
  globals.Register("echolfall",   messageLineAll);
}


int vLuaConsole::message(LuaState *lua, HCONSOLE console, const char *sep) {
  char str[1024] = "";
  int pos = 0;
  int seplen = (int)VSTRLEN(sep);
  int top = lua->GetTop();

  for (int i=1; i<=top; i++) {
    if (lua->Stack(i).IsString()) {
      const char *lua_string = lua->Stack(i).GetString();
      int len = (int)VSTRLEN(lua_string);

      VSTRCAT(str, lua_string, sizeof(str)-pos);
      pos += len;

      if (i != top) {
        VSTRCAT(str, sep, sizeof(str)-pos);
        pos += seplen;
      }
    }
  }
  console->print(str);
  return 0;
}


#endif//_RMX_LUA_
