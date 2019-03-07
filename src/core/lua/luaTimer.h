/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __LUATIMER_H__
#define __LUATIMER_H__


#include "luaHelper.h"
#ifdef _RMX_LUA_


#include "../../../sdk/Win32/Timer.h"


/*
This class manages all of the Timer objects in LUA as well as the "timer" table
*/


class vLuaTimer : public vTimer {
  public:
    vLuaTimer(const char *command, int timeout, BOOL start=FALSE);
    virtual ~vLuaTimer();

  public:
    static void init(LuaObject globals);

    inline const char *getCommand() const { return script.getName(); }

    void resetTimer();
    inline int getTicks() const { return ticks; }

    virtual void onTimer(DWORD systemTime);

    static void stopAllTimers();

    static vLuaTimer *getLuaTimer(LuaState *lua, BOOL pushSuccess=TRUE);

    static int _create(LuaState *lua);
    static int _delete(LuaState *lua);
    static int _start(LuaState *lua);
    static int _stop(LuaState *lua);
    static int _getCommand(LuaState *lua);
    static int _getTimeout(LuaState *lua);
    static int _getTicks(LuaState *lua);
    static int _isStarted(LuaState *lua);
    static int _resetTimer(LuaState *lua);
    static int _resetTimeout(LuaState *lua);

  private:
    rmxName script;
    int ticks;
    static rmxList<vLuaTimer*> timers;
};


#endif //_RMX_LUA_


#endif //__LUATIMER_H__
