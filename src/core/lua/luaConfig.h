/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __LUACONFIG_H__
#define __LUACONFIG_H__


#include "luaHelper.h"
#ifdef _RMX_LUA_


#include "../../../sdk/cfg/config.h"


class vLuaConfigTree : public cfgTree {
  public:
    vLuaConfigTree(cfgBase *parent);
    virtual ~vLuaConfigTree();
};


class vLuaReset : public cfgButton {
  public:
    vLuaReset(cfgBase *parent);
    virtual ~vLuaReset();

  protected:
    virtual void onClick(HPLUGIN source);
};



#endif //_RMX_LUA_


#endif //__LUACONFIG_H__
