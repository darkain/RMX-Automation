/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#pragma warning (disable : 4786)


#ifndef __LUAHELPER_H__
#define __LUAHELPER_H__


#include "../../../sdk/rmxBase.h"
#ifdef _RMX_LUA_


#include "../../../libraries/luaplus/Src/LuaPlus/LuaPlus.h"

using namespace LuaPlus;


void lua_create();
void lua_destroy();
void lua_start();
void lua_script(const char *text);

int lua_openLibs(LuaState *lua);


LuaState *getLuaHandle();


#endif //_RMX_LUA_


#endif //__LUAHELPER_H__
