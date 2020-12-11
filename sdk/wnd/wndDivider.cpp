/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "wndDivider.h"


wndDivider::wndDivider(wndBase *parent) : wndBase("wndDivider", parent) {
  insertHookWnd(this);

  mousefocus = FALSE;

  oldx = 0;
  minx = 96;
  maxx = 296;


  setWindowStyleEx(WS_EX_DLGMODALFRAME);
  setBackgroundBrush(GetSysColorBrush(COLOR_BTNFACE));

  whkMouse::setCursor(LoadCursor(NULL, IDC_SIZEWE));
}

wndDivider::~wndDivider() {
}


void wndDivider::onLeftDown(int x, int y) {
  whkMouse::onLeftDown(x, y);
  SetCapture(getWnd());
  mousefocus = TRUE;
  oldx = x;
}

void wndDivider::onLeftUp(int x, int y) {
  whkMouse::onLeftUp(x, y);
  ReleaseCapture();
  mousefocus = FALSE;
}


void wndDivider::onMouseMove(int x, int y, HWND hwnd) {
  whkMouse::onMouseMove(x, y, hwnd);
  if (!mousefocus) return;

  int x2 = getX() + (x - oldx);
  x2 = min(x2, maxx);
  x2 = max(x2, minx);
  if (getX() == x2) return;
  move(x2, 0, FALSE);
  getvParent()->fakeResize();

  RECT rect;
  getWindowRect(&rect);
  RedrawWindow(getWnd(), NULL, NULL, RDW_FRAME|RDW_INVALIDATE);
}


BOOL wndDivider::wndHookProc(whkInfo *info) {
  if (info->msg == WM_NCHITTEST) {
    VUINT hit = DefWindowProc(getWnd(), info->msg, info->param1, info->param2);
    if (hit == HTBORDER) {
      info->ret = HTCLIENT;
      return TRUE;
    }
  }
  return whkMouse::wndHookProc(info);
}


#endif
