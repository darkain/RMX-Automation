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
#include "luaTimer.h"


rmxList<vLuaTimer*> vLuaTimer::timers;


vLuaTimer::vLuaTimer(const char *command, int timeout, BOOL start) : vTimer(timeout, start), script(command) {
  ticks = 0;
  timers.appendItem(this);
}


vLuaTimer::~vLuaTimer() {
  timers.removeItem(this);
}


void vLuaTimer::init(LuaObject globals) {
  LuaObject timer = globals.CreateTable("timer");
  timer.Register("create",       _create);
  timer.Register("delete",       _delete);
  timer.Register("start",        _start);
  timer.Register("stop",         _stop);
  timer.Register("getCommand",   _getCommand);
  timer.Register("getTimeout",   _getTimeout);
  timer.Register("getTicks",     _getTicks);
  timer.Register("isStarted",    _isStarted);
  timer.Register("resetTimer",   _resetTimer);
  timer.Register("resetTimeout", _resetTimeout);
}


void vLuaTimer::resetTimer() {
  ticks = 0;
  resetTimeout();
}


void vLuaTimer::onTimer(DWORD systemTime) {
  ticks++;
  lua_script(getCommand());
}


void vLuaTimer::stopAllTimers() {
  vLuaTimer *timer = timers.getFirstItem();
  while (timer) {
    delete timer;
    timers.removeItem(timer);
    timer = timers.getFirstItem();
  }
}


vLuaTimer *vLuaTimer::getLuaTimer(LuaState *lua, BOOL pushSuccess) {
  if (lua->GetTop() == 1  &&  lua->Stack(1).IsLightUserData()) {
    vLuaTimer *timer = (vLuaTimer*) lua->Stack(1).GetLightUserData();
    if (timers.hasItem(timer)) {
      if (pushSuccess) lua->PushInteger(1);
      return timer;
    }
  }
  lua->PushNil();
  return NULL;
}


int vLuaTimer::_create(LuaState *lua) {
  if (lua->GetTop() > 1) {
    if (lua->Stack(1).IsString()  &&  lua->Stack(2).IsInteger()) {
      const char *command = lua->Stack(1).GetString();
      if (command  &&  *command) {
        int timeout = lua->Stack(2).GetInteger();
        if (timeout > 0) {
          BOOL start = FALSE;
          if (lua->GetTop() > 2  &&  lua->Stack(3).IsBoolean()) start = lua->Stack(3).GetBoolean();

          vLuaTimer *timer = new vLuaTimer(command, timeout, start);
          lua->PushLightUserData(timer);
          return 1;
        }
      }
    }
  }

  lua->PushNil();
  return 1;
}


int vLuaTimer::_delete(LuaState *lua) {
  vLuaTimer *timer = getLuaTimer(lua);
  delete timer;
  return 1;
}


int vLuaTimer::_start(LuaState *lua) {
  vLuaTimer *timer = getLuaTimer(lua);
  if (timer) timer->startTimer();
  return 1;
}


int vLuaTimer::_stop(LuaState *lua) {
  vLuaTimer *timer = getLuaTimer(lua);
  if (timer) timer->stopTimer();
  return 1;
}


int vLuaTimer::_getCommand(LuaState *lua) {
  vLuaTimer *timer = getLuaTimer(lua);
  if (timer) lua->PushString(timer->getCommand());
  return 1;
}


int vLuaTimer::_getTimeout(LuaState *lua) {
  vLuaTimer *timer = getLuaTimer(lua);
  if (timer) lua->PushInteger(timer->getTimeout());
  return 1;
}


int vLuaTimer::_getTicks(LuaState *lua) {
  vLuaTimer *timer = getLuaTimer(lua);
  if (timer) lua->PushInteger(timer->getTicks());
  return 1;
}


int vLuaTimer::_isStarted(LuaState *lua) {
  vLuaTimer *timer = getLuaTimer(lua);
  if (timer) lua->PushBoolean(timer->isStarted()!=0);
  return 1;
}


int vLuaTimer::_resetTimer(LuaState *lua) {
  vLuaTimer *timer = getLuaTimer(lua, TRUE);
  if (timer) timer->resetTimer();
  return 1;
}


int vLuaTimer::_resetTimeout(LuaState *lua) {
  vLuaTimer *timer = getLuaTimer(lua, TRUE);
  if (timer) timer->resetTimeout();
  return 1;
}


#endif//_RMX_LUA_
