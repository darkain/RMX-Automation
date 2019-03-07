/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "whkMouse.h"
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")


whkMouse::whkMouse(wndBase *parentwnd) : whkBase(parentwnd) {
  hcursor = NULL;
  mouseover = FALSE;
  setReturnDefault(FALSE);
}


whkMouse::~whkMouse() {
  setCursor(NULL);
}


BOOL whkMouse::wndHookProc(whkInfo *info) {
  if (!info) return whkBase::wndHookProc(info);

  switch (info->msg) {
    case WM_LBUTTONDOWN: onLeftDown(  GET_X_LPARAM(info->param2), GET_Y_LPARAM(info->param2)); break;
    case WM_LBUTTONUP:   onLeftUp(    GET_X_LPARAM(info->param2), GET_Y_LPARAM(info->param2)); break;
    case WM_MBUTTONDOWN: onMiddleDown(GET_X_LPARAM(info->param2), GET_Y_LPARAM(info->param2)); break;
    case WM_MBUTTONUP:   onMiddleUp(  GET_X_LPARAM(info->param2), GET_Y_LPARAM(info->param2)); break;
    case WM_RBUTTONDOWN: onRightDown( GET_X_LPARAM(info->param2), GET_Y_LPARAM(info->param2)); break;
    case WM_RBUTTONUP:   onRightUp(   GET_X_LPARAM(info->param2), GET_Y_LPARAM(info->param2)); break;

    case WM_MOUSEMOVE:  onMouseMove(GET_X_LPARAM(info->param2), GET_Y_LPARAM(info->param2), info->wnd->getWnd()); break;
    case WM_MOUSELEAVE: setMouseOver(FALSE); break;

    default: return whkBase::wndHookProc(info);
  }
  return (getReturnDefault()) ? (whkBase::wndHookProc(info)) : (FALSE);
}


void whkMouse::onMouseMove(int x, int y, HWND hwnd) {
  if (hcursor) SetCursor(hcursor);
  setMouseOver(TRUE, TRUE, hwnd);
}


void whkMouse::setMouseOver(BOOL over, BOOL track, HWND hwnd) {
  over = !!over;
  track = !!track;
  if (over == mouseover) return;
  mouseover = over;
  if (mouseover == track == TRUE) trackMouseLeave(hwnd);
  onSetMouseOver(mouseover);
}


void whkMouse::trackMouseLeave(HWND hwnd) {
  TRACKMOUSEEVENT event;
  ZeroMemory(&event, sizeof(TRACKMOUSEEVENT));
  event.cbSize = sizeof(TRACKMOUSEEVENT);
  event.dwFlags = TME_LEAVE;
  event.hwndTrack = hwnd;
  _TrackMouseEvent(&event);
}


void whkMouse::setCursor(HCURSOR cursor) {
  if (hcursor) DestroyCursor(hcursor);
  hcursor = cursor;
}
