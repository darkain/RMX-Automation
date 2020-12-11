/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../../sdk/func/funcGroup.h"
#include "../../sdk/Win32/Win32.h"
#include "uiMouse.h"
#include "uiMouseButton.h"
#include "uiMouseWheel.h"
#include "uiMouseAxis.h"


#ifndef MOUSEEVENTF_HWHEEL  //new to Windows Vista
#  define MOUSEEVENTF_HWHEEL 0x1000
#endif


uiMouse *mouse = NULL;


uiMouse::uiMouse(cfgBase *parent) : cfgTree("UI Mouse", parent) {
  mouse = this;
  unlock();

  funcGroup *group = new funcGroup("Mouse", "mouse");

  x = new uiMouseAxis(1, "Move Horizontal", "mvhorz", group);
  y = new uiMouseAxis(2, "Move Vertical",   "mvvert", group);

  x->setHint("cursorx");
  y->setHint("cursory");

  POINT pt;
  GetCursorPos(&pt);
  setMouseCoords(pt.x, pt.y);

  (new uiMouseButton(MOUSEEVENTF_LEFTUP,   MOUSEEVENTF_LEFTDOWN,          0, "Button 1", "btn1", group))->setHint("action0");
  (new uiMouseButton(MOUSEEVENTF_RIGHTUP,  MOUSEEVENTF_RIGHTDOWN,         0, "Button 2", "btn2", group))->setHint("action1");
  (new uiMouseButton(MOUSEEVENTF_MIDDLEUP, MOUSEEVENTF_MIDDLEDOWN,        0, "Button 3", "btn3", group))->setHint("action2");
  (new uiMouseButton(MOUSEEVENTF_XUP,      MOUSEEVENTF_XDOWN,      XBUTTON1, "Button 4", "btn4", group))->setHint("action3");
  (new uiMouseButton(MOUSEEVENTF_XUP,      MOUSEEVENTF_XDOWN,      XBUTTON2, "Button 5", "btn5", group))->setHint("action4");

  new uiMouseWheel(MOUSEEVENTF_WHEEL,  "Vertical Wheel",   "vwheel", group);
  new uiMouseWheel(MOUSEEVENTF_HWHEEL, "Horizontal Wheel", "hwheel", group);


  //todo: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/windowsuserinterface/windowing/hooks/usinghooks.asp
  if (vWin32::isWin9x()) {
    CONSOLE_ACTIVE->print(getName(), "Windows 9x Not Yet Supported");
    hook = NULL;

  } else {
    CONSOLE_ACTIVE->print(getName(), "Setting up Windows NT Mouse Hook");
    hook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, vWin32::getInstance(), NULL);
  }
}


uiMouse::~uiMouse() {
  lock();

  if (hook) {
    SetLastError(0);
    UnhookWindowsHookEx(hook);
    if (GetLastError()) {
      char str[128] = "";
      VPRINTF(str, sizeof(str), "ERROR CODE: %d : %d", GetLastError(), hook);
      CONSOLE_ACTIVE->printError(getName(), str);
    }
    hook = NULL;
  }

  mouse = NULL;
}


void uiMouse::setMouseCoords(int xpos, int ypos) {
  if (isLocked()) return;
  lock();
  x->setValue(xpos);
  y->setValue(ypos);
  unlock();
}


LRESULT CALLBACK uiMouse::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (wParam == WM_MOUSEMOVE) {
    MSLLHOOKSTRUCT *data = (MSLLHOOKSTRUCT*) lParam;
    if (mouse) mouse->setMouseCoords(data->pt.x, data->pt.y);
  }

  //object can potentially become non-existant from previous call to next
  //so YES, we DO need to test against NULL again

  if (mouse) return CallNextHookEx(mouse->getHook(), nCode, wParam, lParam);
  return 0;
}
