/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_FOREST_H_
#define _WND_FOREST_H_


#include "wndForestBase.h"
#include "../whk/whkScroll.h"


class wndForest : public wndForestBase, public whkScroll {
  public:
    wndForest(wndBase *parent);
    virtual ~wndForest();

  protected:
    int updateScroll();

    virtual void onChildSized(wndForestBase *object);
    virtual int getOffsetX() { return 0; }

    virtual void onInit();
    virtual void onPostInit();
    virtual void onResize(int width, int height);

    virtual void onVertScroll(int pos);

    virtual void onPaint(HDC hdc, PAINTSTRUCT *ps);
    virtual BOOL customPaint() { return TRUE; }
    virtual BOOL eraseBackground(HDC hdc);

    virtual wndBase *getForestWindow() { return scrollwnd; }

    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);

  private:
    int oldw;
    wndLabel *scrollwnd;
};


#endif //_WND_FOREST_H_
