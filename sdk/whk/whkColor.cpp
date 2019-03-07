/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "whkColor.h"


whkColor::whkColor(wndBase *parentwnd) : whkBase(parentwnd) {
}


BOOL whkColor::wndHookProc(whkInfo *info) {
  if (!info) return whkBase::wndHookProc(info);
  wndBase *parent = info->wnd;

  switch (info->msg) {
    case WM_CTLCOLORBTN:
    case WM_CTLCOLORSTATIC:
      if (parent) {
//        char name[256];  //TODO:  why the hell did i heve these here??
//        GetWindowText((HWND)info->param2, name, sizeof(name)-1);
        wndBase *child = parent->getChild((HWND)info->param2);
        if (child) info->ret = (UINT)child->getBackgroundBrush();
        else info->ret = (UINT)parent->getBackgroundBrush();
//        info->ret = (UINT)GetStockObject(HOLLOW_BRUSH);
        return TRUE;
      }
      break;

    case WM_CTLCOLOREDIT:
      info->ret = (UINT)GetSysColorBrush(COLOR_WINDOW);
      return TRUE;
  }

  return whkBase::wndHookProc(info);
}
