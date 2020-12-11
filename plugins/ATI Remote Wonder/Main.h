/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _MAIN_H_
#define _MAIN_H_


#include "../../sdk/plugBase.h"
#include "../../sdk/rmxLock.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/evnt/evntSingle.h"
#include "../../sdk/Win32/MessageWnd.h"
#include "../../sdk/Win32/Timer.h"


#define _CLASS_NAME "ATI RemoteWonder for RMX"


LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


class vAti : public plugBase {
  public:
    vAti();
    virtual ~vAti();

  protected:
    virtual void onInit();
    virtual void onQuit();

  public:
    void rwCommand(int command, BOOL state);
    inline cfgTree *getTree() { return tree; }

  private:
    vMessageWnd *wnd;
    cfgTree *tree;
};


class vRemoteWonderWnd : public vMessageWnd {
  public:
    vRemoteWonderWnd();
    virtual ~vRemoteWonderWnd();

  protected:
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);
};


class vAtiEvent : public evntPress {
  public:
    vAtiEvent(const char *initname, cfgBase *parent, int cmd, const char *hint=NULL)
      : evntPress(initname, parent) {

      insertParamIntNoSave("cmd", cmd);
      setParamReadOnly("cmd", TRUE);
      if (hint) setHint(hint);
    }

    virtual ~vAtiEvent() {}
};


#endif//_MAIN_H_
