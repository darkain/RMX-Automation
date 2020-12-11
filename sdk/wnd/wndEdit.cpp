/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "wndEdit.h"



//WARNING!!!  this should include its own dedicated text buffer!!!
//wndEdit::wndEdit(HWND newWnd) : vSubclassWnd(newWnd) {
//}

//wndEdit::wndEdit(HWND newWnd, wndBase *parent) : vSubclassWnd(newWnd, parent) {
//}

wndEdit::wndEdit(wndBase *parent) : wndSubclass("Edit", parent) {
  setBackgroundBrush(GetSysColorBrush(COLOR_WINDOW));
}


VUINT wndEdit::subWndProc(UINT msg, VUINT param1, VUINT param2) {
  switch (msg) {
//    case WM_SETFOCUS:  notifyParent(vConfigNotify::SETFOCUS, (UINT)hWndmsg, getNotifyId()); break;
//    case WM_KILLFOCUS: notifyParent(vConfigNotify::LOSTFOCUS, (UINT)hWndmsg, getNotifyId()); break;
    case WM_CHAR: if (param1 == VK_RETURN) onEnter(); break;
  }
  return wndSubclass::subWndProc(msg, param1, param2);
}

#endif
