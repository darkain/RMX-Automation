/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "luaHelper.h"
#ifdef _RMX_LUA_


#include "luaConfig.h"
#include "../../../sdk/Win32/Win32.h"


vLuaConfigTree::vLuaConfigTree(cfgBase *parent) : cfgTree("Scripts", parent) {

  new vLuaReset(this);

  {
    char path[MAX_PATH]="";
    VPRINTF(path, sizeof(path), "%s\\rmx_scripts\\", RMXOS::getPath());
    cfgLink *link = new cfgLink(path, this);
    link->setUrl(path);
    link->setSavable(FALSE);
  }

  new cfgLine(this);
}


vLuaConfigTree::~vLuaConfigTree() {
}


//-----------------------------------------


vLuaReset::vLuaReset(cfgBase *parent) : cfgButton("Reset Scripts", parent) {
  setButtonText("Reset");
}


vLuaReset::~vLuaReset() {
}


void vLuaReset::onClick(HPLUGIN source) {
  lua_destroy();
  lua_create();
  lua_start();
}


#endif //_RMX_LUA_
