/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WHK_SCROLL_H_
#define _WHK_SCROLL_H_

#include "whkBase.h"


class whkScroll : public whkBase {
  public:
    whkScroll(wndBase *parentwnd=NULL) : whkBase(parentwnd) {}
    virtual ~whkScroll() {}

  protected:
    //scroll bar functions
    BOOL getHorzScroll();
    void setHorzScroll(BOOL visible);
    int getHorzScrollPos();
    int getHorzScrollMin();
    int getHorzScrollMax();
    int getHorzScrollPage();
    void setHorzScrollPos(int pos);
    void setHorzScrollMinMax(int min, int max);
    void setHorzScrollPage(int page);
    virtual void onHorzScroll(int pos);

    BOOL getVertScroll();
    void setVertScroll(BOOL visible);
    int getVertScrollPos();
    int getVertScrollMin();
    int getVertScrollMax();
    int getVertScrollPage();
    void setVertScrollPos(int pos);
    void setVertScrollMinMax(int min, int max);
    void setVertScrollPage(int page);
    virtual void onVertScroll(int pos);
/*
    int getTrackBarPos();
    int getTrackBarMin();
    int getTrackBarMax();
    int getTrackBarPage();
    void setTrackBarPos(int pos);
    void setTrackBarMin(int min);
    void setTrackBarMax(int max);
    void setTrackBarMinMan(int min, int max);
    void setTrackBarPage(int page);
/*
    BOOL getWndScroll();
    void setWndScroll(BOOL visible);
    int getWndScrollPos();
    int getWndScrollMin();
    int getWndScrollMax();
    int getWndScrollPage();
    void setWndScrollPos(int pos);
    void setWndScrollMinMax(int min, int max);
    void setWndScrollPage(int page);
    virtual void onWndScroll(int pos);*/
    //end scroll bar functions

  protected:
    virtual BOOL wndHookProc(whkInfo *info);
};


#endif //_WHK_SCROLL_H_
