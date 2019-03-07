/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "wndDialog.h"
#include "../whk/whkBase.h"


wndDialog::wndDialog(WORD resource, wndBase *parent) : wndBase((char*)NULL, parent) {
  dlg = resource;
}

wndDialog::~wndDialog() {
}


void wndDialog::setDialogItemText(int resource, const char *text) {
  rmxUtf8ToWide str(text);
  SetWindowText(getDialogItem(resource), str.getBuffer());
}


HWND wndDialog::onCreateWindow() {
  HWND wnd = CreateDialogParam(RMXOS::getInstance(), MAKEINTRESOURCE(dlg), gethParent(), (DLGPROC)vMessageWnd::static_wndProc, (LPARAM)this);
  return wnd;
}


VUINT wndDialog::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == WM_CLOSE) {
    if (flags.deleting) onClose();
    else if (onClose()) delete this;
    return 0;

  } else if (msg == WM_WINDOWPOSCHANGED) {
    if (!getParentWnd()) {
      WINDOWPOS *pos = (WINDOWPOS*)param2;
      if (pos) {
        if (!(pos->flags & SWP_NOMOVE)) updateMoveRect(pos->x,  pos->y,  FALSE);
        if (!(pos->flags & SWP_NOSIZE)) updateSizeRect(pos->cx, pos->cy, FALSE);
      }
    }
  }


  whkInfo hookinfo;
  hookinfo.wnd = this;
  hookinfo.msg = msg;
  hookinfo.param1 = param1;
  hookinfo.param2 = param2;
  hookinfo.ret = 0;

  whkBase *hook = hooks.getFirstItem();
  while (hook) {
    if (hook->wndHookProc(&hookinfo)) return hookinfo.ret;
    hook = hooks.getNextItem(hook);
  }
  
  return hookinfo.ret;
}

#endif
