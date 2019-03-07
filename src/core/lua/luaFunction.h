/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __LUAFUNCTION_H__
#define __LUAFUNCTION_H__


#include "luaHelper.h"
#ifdef _RMX_LUA_


#include "../../../sdk/func/funcSingle.h"


//this class is for running script
//functions from an RMX Event.
class vLuaFunction : public funcSingle {
  public:
    vLuaFunction(cfgRoot *parent);
    virtual ~vLuaFunction() {}

    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


#endif //_RMX_LUA_


#endif //__LUAFUNCTION_H__
