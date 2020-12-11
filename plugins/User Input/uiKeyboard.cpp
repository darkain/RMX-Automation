/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../../sdk/Win32/Win32.h"
#include "../../sdk/wnd/wndBase.h"
#include "../../sdk/cfg/cfgAddChild.h"
#include "uiKeyboard.h"
#include "uiKeyboardFunction.h"


uiKeyboard *keyboard = NULL;


uiKeyboard::uiKeyboard(cfgBase *parent) : uiKeyboardParent("UI Keyboards", parent) {
  keyboard = this;

  wnd    = NULL;
  hook   = NULL;
  active = NULL;
  unlock();

  //TODO: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/windowsuserinterface/windowing/hooks/usinghooks.asp
  if (RMXOS::isWin9x()) {
    CONSOLE_ACTIVE->print(getName(), "Setting up Windows 9x Keyboard Hook");
    wnd = new uiKeyboardWnd();
    wnd->init();
    hook = SetWindowsHookEx(WH_KEYBOARD, win9x_hook, RMXOS::getInstance(), NULL);

  } else {
    CONSOLE_ACTIVE->print(getName(), "Setting up Windows NT Keyboard Hook");
    wnd = NULL;
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, winnt_hook, RMXOS::getInstance(), NULL);
  }

  if (!hook) {
    CONSOLE_ACTIVE->printError(getName(), "Failed to aquire Keyboard Hook");
    return;
  }

  CONSOLE_ACTIVE->print(getName(), "Started Successfully");
  setTitle("System-Wide Keyboard Shortcuts");
  new tcfgAddChildExpand<uiKeyboardEvent>("Add Keyboard Shortcut", "Keyboard Shortcut", this);
  new cfgLine(this);
  new uiKeyboardFunction();
}


uiKeyboard::~uiKeyboard() {
  lock();

  active = NULL;

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

  keyboard = NULL;
}


uiKeyboardEvent *uiKeyboard::getEvent(const char *raw) {
  if (!raw) return NULL;

  cfgBase *child = getFirstUnique();
  while (child) {
    uiKeyboardEvent *event = RMX_CAST<uiKeyboardEvent*>(child);
    if (event) {
      const char *event_raw = event->getRawText();
      if (event_raw  &&  !VSTRCMP(raw, event_raw)) return event;
    }
    child = getNextUnique(child);
  }

  return NULL;
}


BOOL uiKeyboard::hookmsg(WPARAM wParam, DWORD vkCode, DWORD scanCode, DWORD keyFlags) {
  if (isLocked()) return FALSE;

  if ((vkCode == 0) & (scanCode == 0)) return (active!=NULL);

  switch (vkCode) {
    case VK_SHIFT:    case VK_LSHIFT:    case VK_RSHIFT:
    case VK_CONTROL:  case VK_LCONTROL:  case VK_RCONTROL:
    case VK_MENU:     case VK_LMENU:     case VK_RMENU:
    case VK_LWIN:     case VK_RWIN:      case VK_APPS:
      return (active!=NULL);
  }

  BOOL shft = ( (GetAsyncKeyState(VK_SHIFT)   && 0x8000) );
  BOOL ctrl = ( (GetAsyncKeyState(VK_CONTROL) && 0x8000) );
  BOOL menu = ( (GetAsyncKeyState(VK_MENU)    && 0x8000) );
  BOOL wndo = ( ( (GetAsyncKeyState(VK_LWIN)  && 0x8000) ) | ( (GetAsyncKeyState(VK_RWIN) && 0x8000) ) );
  int kFlags = (menu) | (ctrl<<1) | (shft<<2) | (wndo<<3);

  char raw[256] = "";
  VPRINTF(raw, sizeof(raw), "0x%02X%04X%02X", vkCode, scanCode, kFlags);

  if (active) {
    char str[256] = "";
    translateCodes(vkCode, scanCode, kFlags, str);
    active->setText(str, raw);
    return TRUE;
  }

  if ((wParam == WM_KEYDOWN) || (wParam == WM_SYSKEYDOWN)) {
    uiKeyboardEvent *event = getEvent(raw);
    if (event) event->activatePress();
  } else if ((wParam == WM_KEYUP) || (wParam == WM_SYSKEYUP)) {
    uiKeyboardEvent *event = getEvent(raw);
    if (event) event->deactivatePress();
  }

  return FALSE;
}


void uiKeyboard::translateCodes(DWORD vkCode, DWORD scanCode, DWORD keyFlags, char *str) {
  *str = NULL;
  char tmp[32]="";

  if (keyFlags & 0x00000002) VSTRCAT(str, "[CTRL] + ", 256);
  if (keyFlags & 0x00000004) VSTRCAT(str, "[SHIFT] + ", 256);
  if (keyFlags & 0x00000001) VSTRCAT(str, "[ALT] + ", 256);
  if (keyFlags & 0x00000008) VSTRCAT(str, "[WIN] + ", 256);

  VSTRCAT(str, "[", 256);

  switch (vkCode) {
    case 0: VSTRCPY(str, "[NONE]", 7); break;

    case 'A':  case 'B':  case 'C':  case 'D':
    case 'E':  case 'F':  case 'G':  case 'H':
    case 'I':  case 'J':  case 'K':  case 'L':
    case 'M':  case 'N':  case 'O':  case 'P':
    case 'Q':  case 'R':  case 'S':  case 'T':
    case 'U':  case 'V':  case 'W':  case 'X':
    case 'Y':  case 'Z':  case '0':  case '1':
    case '2':  case '3':  case '4':  case '5':
    case '6':  case '7':  case '8':  case '9':
      VPRINTF(tmp, 4, "%c", (vkCode & 0x000000FF));
      VSTRCAT(str, tmp, 256);
      break;

    case VK_F1:   case VK_F2:   case VK_F3:
    case VK_F4:   case VK_F5:   case VK_F6:
    case VK_F7:   case VK_F8:   case VK_F9:
    case VK_F10:  case VK_F11:  case VK_F12:
    case VK_F13:  case VK_F14:  case VK_F15:
    case VK_F16:  case VK_F17:  case VK_F18:
    case VK_F19:  case VK_F20:  case VK_F21:
    case VK_F22:  case VK_F23:  case VK_F24:
      VPRINTF(tmp, sizeof(tmp), "F%d", ((vkCode & 0x000000FF)-VK_F1+1) );
      VSTRCAT(str, tmp, 256);
      break;

    case VK_NUMPAD0:  case VK_NUMPAD1:
    case VK_NUMPAD2:  case VK_NUMPAD3:
    case VK_NUMPAD4:  case VK_NUMPAD5:
    case VK_NUMPAD6:  case VK_NUMPAD7:
    case VK_NUMPAD8:  case VK_NUMPAD9:
      VPRINTF(tmp, sizeof(tmp), "NUMPAD %d", ((vkCode & 0x000000FF)-VK_NUMPAD0) );
      VSTRCAT(str, tmp, 256);
      break;

    case VK_BACK:       VSTRCAT(str, "BACK",        256); break;
    case VK_TAB:        VSTRCAT(str, "TAB",         256); break;
    case VK_CLEAR:      VSTRCAT(str, "CLEAR",       256); break;
    case VK_RETURN:     VSTRCAT(str, "ENTER",       256); break;
    case VK_PAUSE:      VSTRCAT(str, "PAUSE",       256); break;
    case VK_CAPITAL:    VSTRCAT(str, "CAPS LOCK",   256); break;
    case VK_KANA:       VSTRCAT(str, "KANA",        256); break;
    case VK_JUNJA:      VSTRCAT(str, "JUNJA",       256); break;
    case VK_FINAL:      VSTRCAT(str, "FINAL",       256); break;
    case VK_KANJI:      VSTRCAT(str, "KANJI",       256); break;
    case VK_ESCAPE:     VSTRCAT(str, "ESC",         256); break;
    case VK_CONVERT:    VSTRCAT(str, "CLEAR",       256); break;
    case VK_NONCONVERT: VSTRCAT(str, "NON-CONVERT", 256); break;
    case VK_ACCEPT:     VSTRCAT(str, "ACCEPT",      256); break;
    case VK_MODECHANGE: VSTRCAT(str, "MODE CHANGE", 256); break;
    case VK_SPACE:      VSTRCAT(str, "SPACE BAR",   256); break;
    case VK_PRIOR:      VSTRCAT(str, "PAGE UP",     256); break;
    case VK_NEXT:       VSTRCAT(str, "PAGE DOWN",   256); break;
    case VK_END:        VSTRCAT(str, "END",         256); break;
    case VK_HOME:       VSTRCAT(str, "HOME",        256); break;
    case VK_LEFT:       VSTRCAT(str, "LEFT",        256); break;
    case VK_UP:         VSTRCAT(str, "UP",          256); break;
    case VK_RIGHT:      VSTRCAT(str, "RIGHT",       256); break;
    case VK_DOWN:       VSTRCAT(str, "DOWN",        256); break;
    case VK_SELECT:     VSTRCAT(str, "SELECT",      256); break;
    case VK_PRINT:      VSTRCAT(str, "PRINT",       256); break;
    case VK_EXECUTE:    VSTRCAT(str, "EXECUTE",     256); break;
    case VK_SNAPSHOT:   VSTRCAT(str, "SNAP SHOT",   256); break;
    case VK_INSERT:     VSTRCAT(str, "INSERT",      256); break;
    case VK_DELETE:     VSTRCAT(str, "DELETE",      256); break;
    case VK_HELP:       VSTRCAT(str, "HELP",        256); break;
    case VK_APPS:       VSTRCAT(str, "APP MENU",    256); break;
    case VK_MULTIPLY:   VSTRCAT(str, "MULTIPLY",    256); break;
    case VK_ADD:        VSTRCAT(str, "ADD",         256); break;
    case VK_SEPARATOR:  VSTRCAT(str, "SEPARATOR",   256); break;
    case VK_SUBTRACT:   VSTRCAT(str, "SUBTRACT",    256); break;
    case VK_DECIMAL:    VSTRCAT(str, "DECIMAL",     256); break;
    case VK_DIVIDE:     VSTRCAT(str, "DIVIDE",      256); break;
    case VK_NUMLOCK:    VSTRCAT(str, "NUM LOCK",    256); break;
    case VK_SCROLL:     VSTRCAT(str, "SCROLL LOCK", 256); break;
    case VK_PROCESSKEY: VSTRCAT(str, "PROCESS",     256); break;
    case VK_ATTN:       VSTRCAT(str, "ATTN",        256); break;
    case VK_CRSEL:      VSTRCAT(str, "CRSEL",       256); break;
    case VK_EXSEL:      VSTRCAT(str, "EXSEL",       256); break;
    case VK_EREOF:      VSTRCAT(str, "EREOF",       256); break;
    case VK_PLAY:       VSTRCAT(str, "PLAY",        256); break;
    case VK_ZOOM:       VSTRCAT(str, "ZOOM",        256); break;
    case VK_NONAME:     VSTRCAT(str, "ANY KEY",     256); break;
    case VK_SLEEP:      VSTRCAT(str, "SLEEP",       256); break;

    case VK_BROWSER_BACK:        VSTRCAT(str, "BROWSER BACKWARDS",    256); break;
    case VK_BROWSER_FORWARD:     VSTRCAT(str, "BROWSER FORWARDS",     256); break;
    case VK_BROWSER_REFRESH:     VSTRCAT(str, "BROWSER REFRESH",      256); break;
    case VK_BROWSER_STOP:        VSTRCAT(str, "BROWSER STOP",         256); break;
    case VK_BROWSER_SEARCH:      VSTRCAT(str, "BROWSER SEARCH",       256); break;
    case VK_BROWSER_FAVORITES:   VSTRCAT(str, "BROWSER FAVOURITES",   256); break;
    case VK_BROWSER_HOME:        VSTRCAT(str, "BROWSER HOME",         256); break;
    case VK_VOLUME_MUTE:         VSTRCAT(str, "VOLUME MUTE",          256); break;
    case VK_VOLUME_DOWN:         VSTRCAT(str, "VOLUME DOWN",          256); break;
    case VK_VOLUME_UP:           VSTRCAT(str, "VOLUME UP",            256); break;
    case VK_MEDIA_NEXT_TRACK:    VSTRCAT(str, "MEDIA NEXT TRACK",     256); break;
    case VK_MEDIA_PREV_TRACK:    VSTRCAT(str, "MEDIA PREVIOUS TRACK", 256); break;
    case VK_MEDIA_STOP:          VSTRCAT(str, "MEDIA STOP",           256); break;
    case VK_MEDIA_PLAY_PAUSE:    VSTRCAT(str, "MEDIA PLAY/PAUSE",     256); break;
    case VK_LAUNCH_MAIL:         VSTRCAT(str, "LAUNCH MAIL",          256); break;
    case VK_LAUNCH_MEDIA_SELECT: VSTRCAT(str, "LAUNCH MEDIA",         256); break;
    case VK_LAUNCH_APP1:         VSTRCAT(str, "LAUNCH APPLICATION 1", 256); break;
    case VK_LAUNCH_APP2:         VSTRCAT(str, "LAUNCH APPLICATION 2", 256); break;

    case VK_OEM_1:          VSTRCAT(str, "OEM COLON",         256); break;
    case VK_OEM_PLUS:       VSTRCAT(str, "OEM PLUS",          256); break;
    case VK_OEM_COMMA:      VSTRCAT(str, "OEM COMMA",         256); break;
    case VK_OEM_MINUS:      VSTRCAT(str, "OEM MINUS",         256); break;
    case VK_OEM_PERIOD:     VSTRCAT(str, "OEM PERIOD",        256); break;
    case VK_OEM_2:          VSTRCAT(str, "OEM QUESTION MARK", 256); break;
    case VK_OEM_3:          VSTRCAT(str, "OEM TILDA",         256); break;
    case VK_OEM_4:          VSTRCAT(str, "OEM LEFT BRACKET",  256); break;
    case VK_OEM_5:          VSTRCAT(str, "OEM PIPE",          256); break;
    case VK_OEM_6:          VSTRCAT(str, "OEM RIGHT BRACKET", 256); break;
    case VK_OEM_7:          VSTRCAT(str, "OEM QUOTE",         256); break;
    case VK_OEM_CLEAR:      VSTRCAT(str, "OEM CLEAR",         256); break;
    case VK_OEM_FJ_JISHO:   VSTRCAT(str, "OEM JISHO",         256); break;
    case VK_OEM_FJ_MASSHOU: VSTRCAT(str, "OEM MASSHOU",       256); break; 
    case VK_OEM_FJ_TOUROKU: VSTRCAT(str, "OEM TOUROKU",       256); break;
    case VK_OEM_FJ_LOYA:    VSTRCAT(str, "OEM LEFT OYAYUBI",  256); break;
    case VK_OEM_FJ_ROYA:    VSTRCAT(str, "OEM RIGHT OYAYUBI", 256); break;
    case VK_OEM_AX:         VSTRCAT(str, "OEM AX",            256); break;
    case VK_ICO_HELP:       VSTRCAT(str, "ICO HELP",          256); break;
    case VK_ICO_00:         VSTRCAT(str, "ICO 00",            256); break;
    case VK_ICO_CLEAR:      VSTRCAT(str, "ICO CLEAR",         256); break;
    case VK_OEM_RESET:      VSTRCAT(str, "OEM RESET",         256); break;
    case VK_OEM_JUMP:       VSTRCAT(str, "OEM JUMP",          256); break;
    case VK_OEM_PA1:        VSTRCAT(str, "OEM PA1",           256); break;
    case VK_OEM_PA2:        VSTRCAT(str, "OEM PA2",           256); break;
    case VK_OEM_PA3:        VSTRCAT(str, "OEM PA3",           256); break;
    case VK_OEM_WSCTRL:     VSTRCAT(str, "OEM WSCTRL",        256); break;
    case VK_OEM_CUSEL:      VSTRCAT(str, "OEM CURSEL",        256); break;
    case VK_OEM_ATTN:       VSTRCAT(str, "OEM ATTN",          256); break;
    case VK_OEM_FINISH:     VSTRCAT(str, "OEM FINISH",        256); break;
    case VK_OEM_COPY:       VSTRCAT(str, "OEM COPY",          256); break;
    case VK_OEM_AUTO:       VSTRCAT(str, "OEM AUTO",          256); break;
    case VK_OEM_ENLW:       VSTRCAT(str, "OEM ENLW",          256); break;
    case VK_OEM_BACKTAB:    VSTRCAT(str, "OEM BACKTAB",       256); break;
    case VK_PA1:            VSTRCAT(str, "PA1",               256); break;
    case VK_PACKET:         VSTRCAT(str, "PACKET",            256); break;

    default: VSTRCAT(str, "UNKNOWN", 256);
  }

  VSTRCAT(str, "]", 256);
}


LRESULT CALLBACK uiKeyboard::winnt_hook (int nCode, WPARAM wParam, LPARAM lParam) {
  PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT) lParam;
  if (keyboard) {
    if (keyboard->hookmsg(wParam, p->vkCode, p->scanCode, p->flags)) return 1;
    return CallNextHookEx(keyboard->getHook(), nCode, wParam, lParam);
  }
  return 0;
}


LRESULT CALLBACK uiKeyboard::win9x_hook (int nCode, WPARAM wParam, LPARAM lParam) {
  HWND keyboard = FindWindowA("Keyboard Shortcuts", NULL);
  if (keyboard) SendMessage(keyboard, WM_USER+10, wParam, lParam);
//  return CallNextHookEx(APIKBone->getHook(), nCode, wParam, lParam);
  return 0;
}



//--------------------------------------------------------------------




VUINT uiKeyboardWnd::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == WM_USER+10) {
    DWORD vkCode = param1;
    DWORD scanCode = HIBYTE(LOWORD(param2));
    DWORD flags = ((param2>>24) & 0x1L);

    WPARAM wprm = 0;
    if (!(param2 & 0x40000000)) wprm = WM_KEYDOWN;
    else if (param2 & 0x80000000) wprm = WM_KEYUP;

    if (wprm) keyboard->hookmsg(wprm, vkCode, scanCode, flags);
  }
  return 0;
}

