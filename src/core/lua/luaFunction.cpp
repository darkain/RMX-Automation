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
#include "../plugin.h"
#include "luaFunction.h"


vLuaFunction::vLuaFunction(cfgRoot *parent) : funcSingle("Script Command", NULL, parent) {
  funcParam *sub = new funcParam("Command", "f_cmd", "string", this);
}


void vLuaFunction::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  vPlugin *plugin = plg->getPlugin(source);
  if (!plugin) return;
  const char *cmd = plugin->list_getParam(input, "f_cmd");

  DEBUGMSG("Lua", cmd);

  if (cmd) lua_script(cmd);
}


#endif//_RMX_LUA_
