/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CONSOLEWND_H_
#define _CONSOLEWND_H_


#include "../rmx_win32.h"
#include "../../../sdk/wnd/wndBase.h"
#include "../../../sdk/cons/consBase.h"


class vMainConsole : public consBase, public wndBase {
  public:
    vMainConsole(cfgBase *parent);
    virtual ~vMainConsole();

  protected:
    virtual void onNameChange(const char *newname);
    virtual void print(const char *s);

  public:
    void showConsole();
    void hideConsole();

    int ansiToHtml(int color);

    void updateScroll();

    virtual void onInit();

    inline wndBase *getConsoleWnd() { return consolewnd; }

  protected:
    virtual BOOL eraseBackground(HDC hdc) { return TRUE; }
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);

  private:
    HANDLE   buffer;
    HWND     conwnd;
    wndBase *consolewnd;
    wndBase *childwnd;
};


class vNotWnd : public wndBase {
  public:
    vNotWnd(const char *classname, wndBase *parent) : wndBase(classname, parent) {}
    virtual ~vNotWnd() {}
    virtual BOOL customPaint() { return TRUE; }
    virtual BOOL eraseBackground(HDC hdc) { return TRUE; }
};


#endif//_CONSOLEWND_H_
