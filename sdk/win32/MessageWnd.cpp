/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/



#include "../rmxBase.h"
#include "MessageWnd.h"


vMessageWnd::vMessageWnd(const char *Classname, vMessageWnd *parentwnd) : classname(Classname) {
  parent = parentwnd;
  hwnd = NULL;
  hparent = HWND_MESSAGE;
  msgmsg = 0;
  msgparam1 = 0;
  msgparam2 = 0;
}


vMessageWnd::vMessageWnd(const wchar_t *Classname, vMessageWnd *parentwnd) : classname(Classname) {
  parent = parentwnd;
  hwnd = NULL;
  hparent = HWND_MESSAGE;
  msgmsg = 0;
  msgparam1 = 0;
  msgparam2 = 0;
}


vMessageWnd::~vMessageWnd() {
  SetWindowLongPtr(getWnd(), GWLP_USERDATA, (DWORD)NULL);
  DestroyWindow(hwnd);
  hwnd = NULL;
}


HWND vMessageWnd::init() {
  if (hwnd) return NULL;
  hwnd = onCreateWindow();
  if (hwnd) {
    onInit();
    onPostInit();
  } else {
    CONSOLE_MAIN->print("vMessageWnd:init()");
  }
  return hwnd;
}


HWND vMessageWnd::onCreateWindow() {
  rmxUtf8ToWide str(getClassName());

  WNDCLASSEX wc;
  ZeroMemory(&wc, sizeof(WNDCLASSEX));
  if (GetClassInfoEx(RMXOS::getInstance(), str.getBuffer(), &wc) == 0) {
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = (WNDPROC)vMessageWnd::static_wndProc;
    wc.hInstance     = RMXOS::getInstance();
    wc.lpszClassName = str.getBuffer();
    if (!RegisterClassEx(&wc)) {
      CONSOLE_MAIN->print("vMessageWnd:onCreateWindow()");
      return NULL;
    }
  }

  if (parent) hparent = parent->getWnd();
  if (hparent == HWND_MESSAGE) if (!RMXOS::isWin50()) hparent = NULL;

  rmxUtf8ToWide name(*this);

  HWND hwnd = CreateWindowEx(0, str.getBuffer(), name.getBuffer(), 0, 0, 0, 0, 0, hparent, 0, RMXOS::getInstance(), this);

  return hwnd;
}


VUINT vMessageWnd::sendMessage(UINT msg, VUINT param1, VUINT param2) {
  return SendMessage(getWnd(), msg, param1, param2);
}

VUINT vMessageWnd::sendMessageA(UINT msg, VUINT param1, VUINT param2) {
  return SendMessageA(getWnd(), msg, param1, param2);
}

VUINT vMessageWnd::sendMessageW(UINT msg, VUINT param1, VUINT param2) {
  return SendMessageW(getWnd(), msg, param1, param2);
}


VUINT vMessageWnd::postMessage(UINT msg, VUINT param1, VUINT param2) {
  return PostMessage(getWnd(), msg, param1, param2);
}

VUINT vMessageWnd::postMessageA(UINT msg, VUINT param1, VUINT param2) {
  return PostMessageA(getWnd(), msg, param1, param2);
}

VUINT vMessageWnd::postMessageW(UINT msg, VUINT param1, VUINT param2) {
  return PostMessageW(getWnd(), msg, param1, param2);
}


VUINT vMessageWnd::wndProc(UINT msg, VUINT param1, VUINT param2) {
  return DefWindowProc(getWnd(), msg, param1, param2);
}

VUINT vMessageWnd::wndProc_setup(UINT msg, VUINT param1, VUINT param2) {
  msgmsg = msg;
  msgparam1 = param1;
  msgparam2 = param2;
  return wndProc(msg, param1, param2);
}

LRESULT CALLBACK vMessageWnd::static_wndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  if (msg == WM_INITDIALOG) {
    SetWindowLongPtr(hwnd, GWLP_USERDATA, lparam);
  
  } else if (msg == WM_CREATE) {
    CREATESTRUCT *create = (CREATESTRUCT*) lparam;
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (DWORD)create->lpCreateParams);
  }
  
  vMessageWnd *wnd = (vMessageWnd*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  
  return wnd
    ? wnd->wndProc_setup(msg, wparam, lparam)
    : DefWindowProc(hwnd, msg, wparam, lparam);
}