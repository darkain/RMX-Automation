/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "Win32.h"
#include "TrayIcon.h"


UINT vTrayIcon::taskbarmsg = 0;



vTrayIcon::vTrayIcon(HICON icon) : vMessageWnd("vTrayIcon") {
  if (taskbarmsg == 0) taskbarmsg = RegisterWindowMessage(L"TaskbarCreated");

  visible = FALSE;
  ZeroMemory(&icond, sizeof(NOTIFYICONDATA));
  icond.cbSize = sizeof(NOTIFYICONDATA);
  icond.uID = (UINT)this;
  hicon = NULL;
  setIcon(icon);
}

vTrayIcon::~vTrayIcon() {
  removeIcon();
  setIcon((HICON)NULL);
}


void vTrayIcon::updateIcon(BOOL add) {
  if (!getWnd()  ||  !isVisible()) return;
  icond.uFlags = NIF_ICON | NIF_MESSAGE;
  icond.uCallbackMessage = WM_USER+1;
  icond.hIcon = getIcon();
  icond.hWnd = getWnd();

  const char *tooltip = *this;
  if (tooltip) if (*tooltip) {
    icond.uFlags |= NIF_TIP;
    rmxUtf8ToWide str(tooltip);
    WSTRCPY(icond.szTip, str.getBuffer(), sizeof(icond.szTip)/2);
  }

  if (add) Shell_NotifyIcon(NIM_ADD, &icond);
  else Shell_NotifyIcon(NIM_MODIFY, &icond);
  visible = TRUE;
}


void vTrayIcon::removeIcon() {
  Shell_NotifyIcon(NIM_DELETE, &icond);
}


void vTrayIcon::setIcon(HICON icon, BOOL destroy) {
  if (hicon && destroy) DestroyIcon(hicon);
  hicon = icon;
  updateIcon();
}


void vTrayIcon::setIcon(const char *filename) {
  rmxUtf8ToWide str(filename);
  setIcon((HICON)LoadImage(NULL, str.getBuffer(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
}


void vTrayIcon::popBallonTip(const char *title, const char *msg, UINT timeout) {
  if (!visible) return;

  rmxUtf8ToWide titlestr(title);
  rmxUtf8ToWide msgstr(msg);

  icond.uFlags = NIF_INFO;
  WSTRCPY(icond.szInfo, msgstr.getBuffer(), sizeof(icond.szInfo)/2);
  WSTRCPY(icond.szInfoTitle, titlestr.getBuffer(), sizeof(icond.szInfoTitle)/2);
  icond.uTimeout = timeout;
  Shell_NotifyIcon(NIM_MODIFY,&icond);
}


void vTrayIcon::onInit() {
  vMessageWnd::onInit();
  updateIcon(TRUE);
}


void vTrayIcon::setVisible(BOOL vis) {
  vis = !!vis;
  if (vis == visible) return;
  visible = vis;

  if (visible) {
    updateIcon(TRUE);
  } else {
    removeIcon();
  }
}


void vTrayIcon::onNameChange(const char *newname) {
//  vMessageWnd::onNameChange(newname);
  updateIcon();
}


//WM_CONTEXTMENU 
VUINT vTrayIcon::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == WM_USER+1) {
    switch (LOWORD(param2)) {
      case WM_LBUTTONDOWN:    onLeftClick(0, 0);          return 0;
      case WM_LBUTTONDBLCLK:  onLeftDoubleClick(0, 0);    return 0;
      case WM_MBUTTONDOWN:    onMiddleClick(0, 0);        return 0;
      case WM_MBUTTONDBLCLK:  onMiddleDoubleClick(0, 0);  return 0;
      case WM_RBUTTONDOWN:    onRightClick(0, 0);         return 0;
      case WM_RBUTTONDBLCLK:  onRightDoubleClick(0, 0);   return 0;
    }
  } else if (msg == taskbarmsg) {
    updateIcon(isVisible());
  }
  return vMessageWnd::wndProc(msg, param1, param2);
}
