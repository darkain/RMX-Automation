/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_DIVIDER_H_
#define _WND_DIVIDER_H_


#include "wndBase.h"
#include "../whk/whkMouse.h"


class wndDivider : public wndBase, public whkMouse {
  public:
    wndDivider(wndBase *parent);
    virtual ~wndDivider();

    void setMinX(int x) { minx = x; }
    void setMaxX(int x) { maxx = x; }

  protected:
    virtual void onLeftDown(int x, int y);
    virtual void onLeftUp(int x, int y);
    virtual void onMouseMove(int x, int y, HWND hwnd);
//    virtual LRESULT CALLBACK wndProc(HWND hWndmsg, UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual BOOL wndHookProc(whkInfo *info);

  private:
    BOOL mousefocus;
    int oldx;
    int minx, maxx;
};


#endif //_WND_DIVIDER_H_
