/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WHK_MOUSE_H_
#define _WHK_MOUSE_H_


#include "whkBase.h"


class whkMouse : public whkBase {
  public:
    whkMouse(wndBase *parentwnd=NULL);
    virtual ~whkMouse();

  protected:
    virtual void onLeftDown(       int x, int y) {}
    virtual void onLeftUp(         int x, int y) {}
    virtual void onLeftClick(      int x, int y) {}
    virtual void onLeftDoubleClick(int x, int y) {}

    virtual void onMiddleDown(       int x, int y) {}
    virtual void onMiddleUp(         int x, int y) {}
    virtual void onMiddleClick(      int x, int y) {}
    virtual void onMiddleDoubleClick(int x, int y) {}

    virtual void onRightDown(       int x, int y) {}
    virtual void onRightUp(         int x, int y) {}
    virtual void onRightClick(      int x, int y) {}
    virtual void onRightDoubleClick(int x, int y) {}

    virtual void onSetMouseOver(BOOL mouseover) {}
    virtual void onMouseMove(int x, int y, HWND hwnd);

  public:
    void setCursor(HCURSOR cursor);
    inline HCURSOR getCursor() const { return hcursor; }

//    virtual void onMouseEnter(int x, int y, BOOL track=TRUE);
    void setMouseOver(BOOL over, BOOL track=FALSE, HWND hwnd=NULL);
    inline BOOL isMouseOver() const { return mouseover; }
//    virtual void onMouseLeave(int x, int y);
    void trackMouseLeave(HWND hwnd);


  protected:
    virtual BOOL wndHookProc(whkInfo *info);

  private:
    BOOL    mouseover;
    HCURSOR hcursor;
};


#endif //_WHK_MOUSE_H_
