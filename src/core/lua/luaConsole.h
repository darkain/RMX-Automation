/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __LUACONSOLE_H__
#define __LUACONSOLE_H__


#include "luaHelper.h"
#ifdef _RMX_LUA_


#include "../callbacks/cbConsole.h"


class vLuaConsole {
  public:
    vLuaConsole(LuaObject globals);

  private:
    static int message(LuaState *lua, HCONSOLE console, const char *sep="");

    static int message(LuaState *lua)          { return message(lua, CONSOLE_ACTIVE);    }
    static int messageMain(LuaState *lua)      { return message(lua, CONSOLE_MAIN);      }
    static int messageAll(LuaState *lua)       { return message(lua, CONSOLE_BROADCAST); }

    static int messageSpace(LuaState *lua)     { return message(lua, CONSOLE_ACTIVE,    " "); }
    static int messageSpaceMain(LuaState *lua) { return message(lua, CONSOLE_MAIN,      " "); }
    static int messageSpaceAll(LuaState *lua)  { return message(lua, CONSOLE_BROADCAST, " "); }

    static int messageLine(LuaState *lua)      { return message(lua, CONSOLE_ACTIVE   , "\r\n"); }
    static int messageLineMain(LuaState *lua)  { return message(lua, CONSOLE_MAIN     , "\r\n"); }
    static int messageLineAll(LuaState *lua)   { return message(lua, CONSOLE_BROADCAST, "\r\n"); }
};


#endif //_RMX_LUA_


#endif //__LUACONSOLE_H__
