/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __RMXWIN32_MASTERCONSOLE_H__
#define __RMXWIN32_MASTERCONSOLE_H__


#include "../../../sdk/cfg/cfgBool.h"
#include "../../../sdk/cfg/cfgWndHelper.h"
#include "../vConsoleWnd/ConsoleWnd.h"


class vMasterConsoleBool : public cfgWndPos<cfgBool> {
  public:
    vMasterConsoleBool(cfgBase *parent, cfgBase *root)
      : cfgWndPos<cfgBool>("Console Window", parent) {
      lock();
      console = new vMainConsole(root);
      console->init();
      setValue(FALSE);
    }


    virtual ~vMasterConsoleBool() {
      vdelnull(console);
    }


  protected:
    virtual void onSetUserData(const prmBase *userdata) {
      wndBase *wnd = getFirstHookWnd();
      if (wnd) wnd->setVisible( userdata->getValueInt() );
//TODO: FIX THIS SHIT!!
//      cfgWndPos<cfgBool>::onSetUserData(userdata);
    }


    virtual BOOL wndHookProc(whkInfo *info) {
      if (info->msg == WM_CLOSE) {
        setValue(FALSE);
        return TRUE;
      }
      return cfgWndPos<cfgBool>::wndHookProc(info);
    }

/*
    virtual void onLoadSettings() {
      cfgWndPos<cfgBool>::onLoadSettings();
      unlock();
      if (!hasHooks()) insertHookWnd(console);
      getFirstHookWnd()->setVisible(getValue());
    }
*/

    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
      if (message == GENERIC_OPEN) {
        setValue(TRUE);
      } else if (message == GENERIC_CLOSE) {
        setValue(FALSE);
      }
      return cfgWndPos<cfgBool>::onMessage(source, message, param1, param2);
    }


  private:
    vMainConsole *console;
};


#endif //__RMXWIN32_MASTERCONSOLE_H__
