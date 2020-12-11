/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_SCROLL_H_
#define _WND_SCROLL_H_
/*

#include "wndBase.h"


class vScrollWnd : public wndBase {
  public:
    vScrollWnd(wndBase *parent);
    virtual ~vScrollWnd();

    virtual void onScroll(int newpos) {}
    virtual void onScrollEnd() {}

    virtual void onInit();
    virtual void onResize(int width, int height);

    void scrollUp(UINT amt=1);
    void scrollDown(UINT amt=1);
  
    void scrollPageUp(UINT amt=1);
    void scrollPageDown(UINT amt=1);

    void scrollToMin();
    void scrollToMax();

    UINT getPageSize();
    void setPageSize(UINT size);

    void setMinMax(int min, int max);
    void setMin(int min);
    void setMax(int max);
    int getMin();
    int getMax();

    int getScrollPosition();
    void setScrollPosition(int pos, BOOL redraw=TRUE);

    virtual UINT wndProc(UINT msg, UINT param1, UINT param2);

  private:
    wndBase *scrollbar;
    SCROLLINFO info;
};
*/


#endif //_WND_SCROLL_H_
