/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _APPCOMMAND_H_
#define _APPCOMMAND_H_


#include "../../sdk/plugBase.h"
#include "../../sdk/rmxLock.h"
#include "../../sdk/Win32/Timer.h"
#include "../../sdk/wnd/wndBase.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/evnt/evntSingle.h"
#include "../../sdk/func/funcSingle.h"
#include "../../sdk/func/funcGroup.h"


class vAppCmdWnd;


class vAppCommand : public plugBase, public rmxLock {
  public:
    vAppCommand();
    virtual ~vAppCommand();

  protected:
    virtual void onInit();
    virtual void onQuit();

  public:
    void addCommand(evntSingle *event) { cmds.appendItem(event); }
    evntSingle *getEvent(int cmd);

    inline vAppCmdWnd *getWindow() { return wnd; }
    inline HHOOK getHook() { return hook; }

    void event(short ID);

  private:
    HHOOK       hook;
    cfgTree    *tree;
    vAppCmdWnd *wnd;
    rmxList<evntSingle*> cmds;
};


class vAppCmdWnd : public vTimer, public wndBase {
  public:
    vAppCmdWnd();
    virtual ~vAppCmdWnd();

  public:
    UINT cmdProc(int code, UINT param1, UINT param2);

  protected:
    virtual void onTimer(DWORD systemTime) { wait = FALSE; }
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);

  private:
    BOOL wait;
};


class vAppEvent : public evntSingle {
  public:
    vAppEvent(const char *name, cfgBase *par, int cmd);
    virtual ~vAppEvent() {}
};


class vAppFunction : public funcSingle, public vTimer {
  public:
    vAppFunction(const char *function_name, const char *script_name, int command, funcGroup *parent);
    virtual ~vAppFunction();

  protected:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
    virtual void onTimer(DWORD systemTime);

  private:
    int cmd;
};


#endif //_APPCOMMAND_H_
