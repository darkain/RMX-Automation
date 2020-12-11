/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/
#if 0


#include "../../../sdk/rmxBase.h"
#include "../../../sdk/wnd/wndBase.h"
#include <time.h>

#include <shobjidl.h>

#include "ConsoleWnd.h"
#include "../../core/Loader.h"
#include "../../stand_alone/resource.h"


vMainConsole::vMainConsole(cfgBase *parent) : consBase("RMX Console", parent), wndBase("RMX Console") {
  wndBase::setName(wndBase::getClassName());
  conwnd     = NULL;
  childwnd   = NULL;
  buffer     = NULL;
  consolewnd = NULL;

  //TODO: see if i even really need this, considering newer wndBase auto-params
  setWindowStyle(WS_VSCROLL | WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX | WS_OVERLAPPED);

#ifndef _RMX_MEDIA_REMOTE_
  char path[MAX_PATH];
  VSTRCPY(path, RMXOS::getPath(), sizeof(path));
  VSTRCAT(path, "\\rmx_docs\\log.html", sizeof(path));

  FILE *file = NULL;
  VFOPEN(&file, path, "w");
  if (!file) return;
  fprintf(file, "<html><body style=\"color:bbbbbb; background-color:black;\"><pre style=\"font-family:monospace;\">\n");
  fclose(file);
#endif //_RMX_MEDIA_REMOTE_
}


vMainConsole::~vMainConsole() {
//  SetConsoleCtrlHandler(HandlerRoutine, FALSE);
  CloseHandle(buffer);
  buffer = NULL;
  FreeConsole();

#ifndef _RMX_MEDIA_REMOTE_
  {
    char path[MAX_PATH];
    VSTRCPY(path, RMXOS::getPath(), sizeof(path));
    VSTRCAT(path, "\\rmx_docs\\log.html", sizeof(path));

    FILE *file = NULL;
    VFOPEN(&file, path, "a+");
    if (!file) return;
    fprintf(file, "</pre></body></html>");
    fclose(file);
  }
#endif //_RMX_MEDIA_REMOTE_
}



void vMainConsole::onInit() {
  wndBase::onInit();
  setIcon(RMX_ICON_LARGE_XP);

  childwnd = new vNotWnd("RMX Console", this);
  childwnd->moveresize(0, 0, 2048, 2048);
  childwnd->init();
  childwnd->showWindow();

  consolewnd = new vNotWnd("RMX Console", this);
  consolewnd->moveresize(0, 0, 2048, 300);
  consolewnd->init();
  consolewnd->showWindow();

  AllocConsole();

  wchar_t tmpstr[64] = L"";
  WPRINTF(tmpstr, sizeof(tmpstr)/2, L"RMX Console %08X", rand());
  SetConsoleTitle(tmpstr);
  conwnd = FindWindow(NULL, tmpstr);
  while (!conwnd) {
    Sleep(0);
    conwnd = FindWindow(NULL, tmpstr);
  }

  ShowWindowAsync(conwnd, SW_HIDE);

  COORD coord;
  coord.X = 80;
  coord.Y = 100;
  buffer = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
  SetConsoleScreenBufferSize(buffer, coord);
  SetConsoleActiveScreenBuffer(buffer);

  SetParent(conwnd, consolewnd->getWnd());
  SetWindowPos(conwnd, NULL, 0, 0, 2048, 300, 0);

  POINT pt1 = {0,0};
  POINT pt2 = {0,0};
  ClientToScreen(getWnd(), &pt1);
  ClientToScreen(conwnd, &pt2);

  SetWindowPos(conwnd, NULL, (pt1.x - pt2.x + 1), (pt1.y - pt2.y + 1), 2048, 400, 0);
  ShowWindowAsync(conwnd, SW_SHOW);

  RECT consolerect;
  GetClientRect(conwnd, &consolerect);
  consolewnd->moveresize(0, 0, consolerect.right, consolerect.bottom);

  
  int swidth = GetSystemMetrics(SM_CXVSCROLL);

  RECT winrect = {0, 0, consolerect.right+swidth, consolerect.bottom};
  int ret = AdjustWindowRectEx(&winrect, getWindowStyle()|WS_VISIBLE, FALSE, getWindowStyleEx());
  resize(winrect.right-winrect.left, winrect.bottom-winrect.top);

  updateScroll();
}



void vMainConsole::print(const char *s) {
  if (!s) return;
  if (!*s) return;
  const char *text = s;
  unsigned long amt = 0;

  bool ansi = false;
  const char *p = s;
  int code = 0;
  int count = 0;
  int color = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;

  FILE *file = NULL;

#ifndef _RMX_MEDIA_REMOTE_
  char path[MAX_PATH];
  VSTRCPY(path, RMXOS::getPath(), sizeof(path));
  VSTRCAT(path, "\\rmx_docs\\log.html", sizeof(path));
  VFOPEN(&file, path, "a+");

  while (1) {
    if (*p == NULL) {
      if (p-s > 0) {
        rmxUtf8ToWide out(s, (int)(p-s));
        WriteConsole(buffer, out.getBuffer(), out.getLength(), &amt, 0);
        if (file) fwrite(s, 1, p-s, file);
      }
      break;
    }
    
    if (ansi) {
      if ( (*p == 'm') || (*p == ';') ) {
        switch (code) {
          case 0:  color  = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE; break;
          case 1:  color |= FOREGROUND_INTENSITY; break;
          case 22: color &= 0xfff7; break;
          //foreground
          case 30: color = (color & 0xfff8) | 0; break; //black
          case 31: color = (color & 0xfff8) | FOREGROUND_RED; break; //red
          case 32: color = (color & 0xfff8) | FOREGROUND_GREEN; break; //green
          case 33: color = (color & 0xfff8) | FOREGROUND_RED|FOREGROUND_GREEN; break; //yellow
          case 34: color = (color & 0xfff8) | FOREGROUND_BLUE; break; //blue
          case 35: color = (color & 0xfff8) | FOREGROUND_RED|FOREGROUND_BLUE; break; //magenta (purple)
          case 36: color = (color & 0xfff8) | FOREGROUND_GREEN|FOREGROUND_BLUE; break; //cyan
          case 37: color = (color & 0xfff8) | FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE; break; //white
          case 39: color = (color & 0xfff8) | FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE; break; //default (white)
          //background
          case 40: color = (color & 0xff8f) | 0; break; //black
          case 41: color = (color & 0xff8f) | BACKGROUND_RED; break; //red
          case 42: color = (color & 0xff8f) | BACKGROUND_GREEN; break; //green
          case 43: color = (color & 0xff8f) | BACKGROUND_RED|BACKGROUND_GREEN; break; //yellow
          case 44: color = (color & 0xff8f) | BACKGROUND_BLUE; break; //blue
          case 45: color = (color & 0xff8f) | BACKGROUND_RED|BACKGROUND_BLUE; break; //magenta (purple)
          case 46: color = (color & 0xff8f) | BACKGROUND_GREEN|BACKGROUND_BLUE; break; //cyan
          case 47: color = (color & 0xff8f) | BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE; break; //white
          case 49: color = (color & 0xff8f) | BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE; break; //default (white)
        }
        code = 0;
      }

      if (*p == 'm') {
        SetConsoleTextAttribute(buffer, color);
        if (file) fprintf(file, "<span style=\"color:%06x; background-color:%06x;\">", ansiToHtml(color), ansiToHtml(color>>4));
        count++;
        ansi = false;
        s = p + 1;
      }

      else if (*p == '[') {
      }  //do nothing

      else if (*p == ';') {
      }  //do nothing

      else if ( (*p >= '0') && (*p <= '9') ) {
        code = (code * 10) + (*p - '0');
      }

      else {
        ansi = false;
      }
    }

    else {
      if (*p == '\033') { //start of ANSI block
        if (p-s > 0) {
          rmxUtf8ToWide out(s, (int)(p-s));
          WriteConsole(buffer, out.getBuffer(), out.getLength(), &amt, 0);
          if (file) fwrite(s, 1, p-s, file);
        }
        s = p;
        ansi = true;
        code = 0;
      }
    }

    p++;
  }
#endif //_RMX_MEDIA_REMOTE_

  WriteConsole(buffer, L"\r\n", 2, &amt, 0);
  SetConsoleTextAttribute(buffer, FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

  if (file) {
    for (int i=0; i<count; i++) {
      fprintf(file, "</span>");
    }
    fprintf(file, "\n");
    fclose(file);
  }

  updateScroll();
}


void vMainConsole::showConsole() {
  showWindow();
  setForeground();
}


void vMainConsole::hideConsole() {
  hideWindow();
}


void vMainConsole::onNameChange(const char *newname) {
  rmxUtf8ToWide str(newname);
  SetConsoleTitle(str.getBuffer());
  consBase::onNameChange(newname);
}


int vMainConsole::ansiToHtml(int color) {
  switch (color & 0x0f) {
    //normal
    case 0x0: return 0x000000;
    case 0x1: return 0x000077;
    case 0x2: return 0x007700;
    case 0x3: return 0x007777;
    case 0x4: return 0x770000;
    case 0x5: return 0x770077;
    case 0x6: return 0x777700;
    case 0x7: return 0xbbbbbb;
    //intense
    case 0x8: return 0x777777;
    case 0x9: return 0x0000ff;
    case 0xA: return 0x00ff00;
    case 0xB: return 0x00ffff;
    case 0xC: return 0xff0000;
    case 0xD: return 0xff00ff;
    case 0xE: return 0xffff00;
    case 0xF: return 0xffffff;
  }
  return 0xbbbbbb;
}


VUINT vMainConsole::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == WM_VSCROLL  ||  msg == WM_MOUSEWHEEL) {
    SendMessage(conwnd, msg, param1, param2);
    updateScroll();
  }
  return wndBase::wndProc(msg, param1, param2);
}


void vMainConsole::updateScroll() {
  SCROLLINFO info;
  info.cbSize = sizeof(info);
  info.fMask = SIF_ALL;
  GetScrollInfo(conwnd, SB_VERT, &info);
  SetScrollInfo(getWnd(), SB_VERT, &info, TRUE);
}

#endif
