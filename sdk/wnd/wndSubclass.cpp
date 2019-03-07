/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "wndSubclass.h"
#include "../whk/whkBase.h"




wndSubclass::wndSubclass(const char *classname, wndBase *parent) : wndBase(classname, parent) {
  prewndBaseProc = NULL;
}


wndSubclass::wndSubclass(const wchar_t *classname, wndBase *parent) : wndBase(classname, parent) {
  prewndBaseProc = NULL;
}


wndSubclass::~wndSubclass() {
  SetWindowLongPtr(getWnd(), GWLP_USERDATA, (LONG)((wndBase*)this));
  if (prewndBaseProc) {
    SetWindowLongPtr(getWnd(), GWLP_WNDPROC, (LONG)prewndBaseProc);
  }
}


void wndSubclass::onInit() {
  wndBase::onInit();
  subclassWindow();
}


void wndSubclass::subclassWindow() {
  SetWindowLongPtr(getWnd(), GWLP_USERDATA, (LONG)this);
  prewndBaseProc = (WNDPROC)SetWindowLongPtr(getWnd(), GWLP_WNDPROC, (LONG)static_subWndProc);
}

VUINT wndSubclass::subWndProc(UINT msg, VUINT param1, VUINT param2) {
  switch (msg) {
    case WM_SETFOCUS:  onSetFocus();  break;
    case WM_KILLFOCUS: onLoseFocus(); break;

    case WM_WINDOWPOSCHANGED: {
      WINDOWPOS *pos = (WINDOWPOS*)param2;
      if (!pos) break;
      if (!(pos->flags & SWP_NOMOVE)) updateMoveRect(pos->x,  pos->y,  FALSE);
      if (!(pos->flags & SWP_NOSIZE)) updateSizeRect(pos->cx, pos->cy, FALSE);
    } break;
  }

  whkInfo hookinfo;
  hookinfo.wnd    = this;
  hookinfo.msg    = msg;
  hookinfo.param1 = param1;
  hookinfo.param2 = param2;
  hookinfo.ret    = 0;

  whkBase *hook = hooks.getFirstItem();
  while (hook) {
    if (hook->wndHookProc(&hookinfo)) return hookinfo.ret;
    hook = hooks.getNextItem(hook);
  }

  return CallWindowProc(prewndBaseProc, getWnd(), msg, param1, param2);
}


LRESULT CALLBACK wndSubclass::static_subWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  wndSubclass *wnd = (wndSubclass*)GetWindowLong(hWnd, GWLP_USERDATA);
  if (!wnd) return DefWindowProc(hWnd, uMsg, wParam, lParam);
  return wnd->subWndProc(uMsg, wParam, lParam);
}



#endif
