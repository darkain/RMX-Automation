/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __RMXWIN32_MASTERWND_H__
#define __RMXWIN32_MASTERWND_H__


#include "../../../sdk/cfg/cfgBool.h"
#include "../../../sdk/cfg/cfgWndHelper.h"

#include "../vConfigWnd/ConfigWnd.h"


class vMasterWndBool : public cfgWndSizePos<cfgBool> {
  public:
    vMasterWndBool(cfgBase *parent)
      : cfgWndSizePos<cfgBool>("Configuration Window", parent) {
      //lock();
      setValue(TRUE);
    }


    virtual ~vMasterWndBool() {}


  protected:
    virtual void onSetUserData(const prmBase *userdata) {
      if (getValue()  &&  isUnlocked()) {
        wndBase *wnd = vConfigWnd::config_create();
        if (wnd) {
          insertHookWnd(wnd);
          wnd->showWindow();
        }
      } else {
        vConfigWnd::config_destroy();
      }
//TODO: FIX THIS SHIT!!
//     cfgWndSizePos<cfgBool>::onSetUserData(userdata);
    }


    virtual void onFirstLoad() {
      setValue(TRUE);
      cfgWndSizePos<cfgBool>::onFirstLoad();
    }

/*
    virtual void onLoadSettings() {
      cfgWndSizePos<cfgBool>::onLoadSettings();
      unlock();
      if (getValue()) {
        wndBase *wnd = vConfigWnd::config_create();
        if (wnd) {
          insertHookWnd(wnd);
          wnd->showWindow();
        }
      }
    }
*/

    virtual BOOL wndHookProc(whkInfo *info) {
      if (info->msg == WM_CLOSE) {
        setValue(FALSE);
        return TRUE;
      }
      return cfgWndSizePos<cfgBool>::wndHookProc(info);
    }


    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
      if (message == GENERIC_OPEN) {
        setValue(TRUE);
      } else if (message == GENERIC_CLOSE) {
        setValue(FALSE);
      }
      return cfgWndSizePos<cfgBool>::onMessage(source, message, param1, param2);
    }
};


#endif //__RMXWIN32_MASTERWND_H__
