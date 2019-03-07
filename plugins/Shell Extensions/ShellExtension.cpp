/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

/*
SetLastError
GetLastError
SetWindowLongPtr
GetWindowLongPtr
EnableWindow
IsWindowEnabled
SetForegroundWindow
SetFocus
IsIconic
IsZoomed
ShowWindow
UpdateWindow
GetWindowText
GetWindowTextLength
SetWindowPos
IsWindowVisible
GetInstance (RMX Specific)

something for getting the focus and foreground windows
*/

#define _BETA

#include "ShellExtension.h"


static vShellExtension *SHELL = new vShellExtension();


vShellExtension::vShellExtension() : plugBase("Shell Extensions") {
}


void vShellExtension::onInit() {
  new FindWindowScript(   this);
  new FindWindowExScript( this);
  new SendMessageScript(  this);
  new PostMessageScript(  this);
  new GetWindowTextScript(this);

  vHFUNCTION::global_setValueInteger("WM_COMMAND",       WM_COMMAND);
  vHFUNCTION::global_setValueInteger("WM_USER",          WM_USER);
  vHFUNCTION::global_setValueInteger("WM_SYSCOMMAND",    WM_SYSCOMMAND);
  vHFUNCTION::global_setValueInteger("WM_KEYDOWN",       WM_KEYDOWN);
  vHFUNCTION::global_setValueInteger("WM_KEYUP",         WM_KEYUP);
  vHFUNCTION::global_setValueInteger("WM_CHAR",          WM_CHAR);
  vHFUNCTION::global_setValueInteger("WM_SYSKEYDOWN",    WM_SYSKEYDOWN);
  vHFUNCTION::global_setValueInteger("WM_SYSKEYUP",      WM_SYSKEYUP);
  vHFUNCTION::global_setValueInteger("WM_SYSCHAR",       WM_SYSCHAR);
  vHFUNCTION::global_setValueInteger("WM_LBUTTONDOWN",   WM_LBUTTONDOWN);
  vHFUNCTION::global_setValueInteger("WM_LBUTTONUP",     WM_LBUTTONUP);
  vHFUNCTION::global_setValueInteger("WM_LBUTTONDBLCLK", WM_LBUTTONDBLCLK);
  vHFUNCTION::global_setValueInteger("WM_RBUTTONDOWN",   WM_RBUTTONDOWN);
  vHFUNCTION::global_setValueInteger("WM_RBUTTONUP",     WM_RBUTTONUP);
  vHFUNCTION::global_setValueInteger("WM_RBUTTONDBLCLK", WM_RBUTTONDBLCLK);
  vHFUNCTION::global_setValueInteger("WM_MBUTTONDOWN",   WM_MBUTTONDOWN);
  vHFUNCTION::global_setValueInteger("WM_MBUTTONUP",     WM_MBUTTONUP);
  vHFUNCTION::global_setValueInteger("WM_MBUTTONDBLCLK", WM_MBUTTONDBLCLK);
  vHFUNCTION::global_setValueInteger("WM_MOUSEWHEEL",    WM_MOUSEWHEEL);
  vHFUNCTION::global_setValueInteger("WM_XBUTTONDOWN",   WM_XBUTTONDOWN);
  vHFUNCTION::global_setValueInteger("WM_XBUTTONUP",     WM_XBUTTONUP);
  vHFUNCTION::global_setValueInteger("WM_XBUTTONDBLCLK", WM_XBUTTONDBLCLK);
  vHFUNCTION::global_setValueInteger("WM_MOUSEHOVER",    WM_MOUSEHOVER);
  vHFUNCTION::global_setValueInteger("WM_MOUSELEAVE",    WM_MOUSELEAVE);
  vHFUNCTION::global_setValueInteger("WM_CUT",           WM_CUT);
  vHFUNCTION::global_setValueInteger("WM_COPY",          WM_COPY);
  vHFUNCTION::global_setValueInteger("WM_PASTE",         WM_PASTE);
  vHFUNCTION::global_setValueInteger("WM_CLEAR",         WM_CLEAR);
  vHFUNCTION::global_setValueInteger("WM_UNDO",          WM_UNDO);
  vHFUNCTION::global_setValueInteger("WM_PRINT",         WM_PRINT);
}


void vShellExtension::win32Message(HPLUGIN source, HLIST input, HLIST output, int method) {
  DWORD msg = NULL;
  HPARAM prmmsg = source->list_getParamHandle(input, "f_msg");
  if (source->param_isString(prmmsg)) {
    const char *value = source->param_getValue(prmmsg);
    if (value) {
      if      (VSTRCMPI(value, "WM_COMMAND"      ) == 0) msg = WM_COMMAND;
      else if (VSTRCMPI(value, "WM_USER"         ) == 0) msg = WM_USER;
      else if (VSTRCMPI(value, "WM_SYSCOMMAND"   ) == 0) msg = WM_SYSCOMMAND;
      else if (VSTRCMPI(value, "WM_KEYDOWN"      ) == 0) msg = WM_KEYDOWN;
      else if (VSTRCMPI(value, "WM_KEYUP"        ) == 0) msg = WM_KEYUP;
      else if (VSTRCMPI(value, "WM_CHAR"         ) == 0) msg = WM_CHAR;
      else if (VSTRCMPI(value, "WM_SYSKEYDOWN"   ) == 0) msg = WM_SYSKEYDOWN;
      else if (VSTRCMPI(value, "WM_SYSKEYUP"     ) == 0) msg = WM_SYSKEYUP;
      else if (VSTRCMPI(value, "WM_SYSCHAR"      ) == 0) msg = WM_SYSCHAR;
      else if (VSTRCMPI(value, "WM_LBUTTONDOWN"  ) == 0) msg = WM_LBUTTONDOWN;
      else if (VSTRCMPI(value, "WM_LBUTTONUP"    ) == 0) msg = WM_LBUTTONUP;
      else if (VSTRCMPI(value, "WM_LBUTTONDBLCLK") == 0) msg = WM_LBUTTONDBLCLK;
      else if (VSTRCMPI(value, "WM_RBUTTONDOWN"  ) == 0) msg = WM_RBUTTONDOWN;
      else if (VSTRCMPI(value, "WM_RBUTTONUP"    ) == 0) msg = WM_RBUTTONUP;
      else if (VSTRCMPI(value, "WM_RBUTTONDBLCLK") == 0) msg = WM_RBUTTONDBLCLK;
      else if (VSTRCMPI(value, "WM_MBUTTONDOWN"  ) == 0) msg = WM_MBUTTONDOWN;
      else if (VSTRCMPI(value, "WM_MBUTTONUP"    ) == 0) msg = WM_MBUTTONUP;
      else if (VSTRCMPI(value, "WM_MBUTTONDBLCLK") == 0) msg = WM_MBUTTONDBLCLK;
      else if (VSTRCMPI(value, "WM_MOUSEWHEEL"   ) == 0) msg = WM_MOUSEWHEEL;
      else if (VSTRCMPI(value, "WM_XBUTTONDOWN"  ) == 0) msg = WM_XBUTTONDOWN;
      else if (VSTRCMPI(value, "WM_XBUTTONUP"    ) == 0) msg = WM_XBUTTONUP;
      else if (VSTRCMPI(value, "WM_XBUTTONDBLCLK") == 0) msg = WM_XBUTTONDBLCLK;
      else if (VSTRCMPI(value, "WM_MOUSEHOVER"   ) == 0) msg = WM_MOUSEHOVER;
      else if (VSTRCMPI(value, "WM_MOUSELEAVE"   ) == 0) msg = WM_MOUSELEAVE;
      else if (VSTRCMPI(value, "WM_CUT"          ) == 0) msg = WM_CUT;
      else if (VSTRCMPI(value, "WM_COPY"         ) == 0) msg = WM_COPY;
      else if (VSTRCMPI(value, "WM_PASTE"        ) == 0) msg = WM_PASTE;
      else if (VSTRCMPI(value, "WM_CLEAR"        ) == 0) msg = WM_CLEAR;
      else if (VSTRCMPI(value, "WM_UNDO"         ) == 0) msg = WM_UNDO;
      else if (VSTRCMPI(value, "WM_PRINT"        ) == 0) msg = WM_PRINT;
    }
  }
  if (msg == NULL) {
    msg = source->param_getValueInt(prmmsg);
  }

  HWND wnd = getWindow(source, input);
  DWORD wprm = (DWORD) source->list_getParamInt(input, "f_wprm");
  DWORD lprm = (DWORD) source->list_getParamInt(input, "f_lprm");

  DWORD ret = 0;
  if (method == 1) {
    ret = SendMessage(wnd, msg, wprm, lprm);
  } else if (method == 2) {
    ret = PostMessage(wnd, msg, wprm, lprm);
  }

  source->list_insertParam( output, "prm1" );
  source->list_setParamInt( output, "prm1", (int)ret );

#if defined _BETA || defined _DEBUG
  {
    char str[1024] = "";
    VPRINTF(str, sizeof(str), "wnd %d : msg %d : wprm %d : lprm %d : ret %d", wnd, msg, wprm, lprm, ret);
    DEBUGMSG(getName(), str);
  }
#endif
}


HWND vShellExtension::getWindow(HPLUGIN source, HLIST input) {
  HPARAM prmwnd = source->list_getParamHandle(input, "f_hwnd");
  if (source->param_isString(prmwnd)) {
    rmxUtf8ToWide str(source->param_getValue(prmwnd));
    return FindWindow(str.getBuffer(), NULL);
  } 
  return (HWND) source->param_getValueInt(prmwnd);
}


//-----------------------------------------------------------------------------------------------


vShellWnd::vShellWnd(const char *name, cfgBase *parent)
 : vMessageWnd(name), cfgTree(name, parent) {
  vMessageWnd::setName(name);
}


vShellWnd::~vShellWnd() {
}


VUINT vShellWnd::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == WM_COMMAND  &&  param1 > 0) {
    cfgBase *child = getFirstChild();
    while (child) {
      if (child->getParamInt("command") == (int)param1) {
        evntSingle *event = static_cast<evntSingle*>(child);
        event->activate();
      }
      child = getNextChild(child);
    }
    return 0;
  }
  return vMessageWnd::wndProc(msg, param1, param2);
}


//-----------------------------------------------------------------------------------------------


void SendMessageScript::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  SHELL->win32Message(source, input, output, 1);
}


//-----------------------------------------------------------------------------------------------


void PostMessageScript::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  SHELL->win32Message(source, input, output, 2);
}


//-----------------------------------------------------------------------------------------------


void GetWindowTextScript::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  PROFILE_STR("GetWindowText");

  source->list_insertParam( output, "prm1" );
  HWND hwnd = SHELL->getWindow(source, input);
  if (hwnd) {
    int len = GetWindowTextLength(hwnd) + 1;
    if (len > 1) {
      wchar_t *str = (wchar_t*) malloc(len * 2);
      GetWindowText(hwnd, str, len);
      rmxToUtf8 tmp(str);
      source->list_setParam(output, "prm1", tmp.getBuffer());  //todo: add support for wide characters directly in cfgBase
      free(str);
    }
  }
}


//-----------------------------------------------------------------------------------------------
/*
vTaskSwitch::vTaskSwitch(const char *name, UINT direction) : vFunction(name) {
  dir = direction;
  pos = 1;
}


UINT vTaskSwitch::onActivated(vFunctionCall *function) {
  wnds.emptyItems();
  EnumWindows(EnumWindowsProc, (LPARAM)this);

  HWND next = wnds.getItem(pos++);
  if (pos > wnds.getItemCount()) pos = 1;
//  HWND next = wnds.getPrevItem(GetForegroundWindow());
//  if (!next) next = wnds.getFirstItem();
  if (IsIconic(next)) ShowWindow(next, SW_RESTORE);
  SetForegroundWindow(next);

  return 0;
}


BOOL CALLBACK vTaskSwitch::EnumWindowsProc(HWND hwnd, LPARAM lParam) {
  if (!lParam) return FALSE;
  return ((vTaskSwitch*)lParam)->enumProc(hwnd);
}


BOOL vTaskSwitch::enumProc(HWND hwnd) {
  if (!IsWindowVisible(hwnd)) return TRUE;
  if (GetParent(hwnd)) return TRUE;
  if (GetWindow(hwnd, GW_OWNER)) return TRUE;
  if (GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) return TRUE;

  char str[4];
  GetWindowText(hwnd, str, sizeof(str)-1);
  str[255] = NULL;
  if (*str == NULL) return TRUE;

  wnds.appendItem(hwnd);
  return TRUE;
}


//-----------------------------------------------------------------------------------------------


UINT vStartMenu::onActivated(vFunctionCall *function) {
  return SendMessage(shell.getRmxAppWindow(), WM_SYSCOMMAND, SC_TASKLIST, 0);
}

*/
