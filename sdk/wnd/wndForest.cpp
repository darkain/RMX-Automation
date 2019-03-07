/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "../rmxBase.h"
#include "wndForest.h"


wndForest::wndForest(wndBase *parent) {
  scrollwnd = new wndLabel(this);
  scrollwnd->setTransparent(TRUE);

  setvParent(parent);
  whkScroll::insertHookWnd(this);
  setBackgroundBrush( GetSysColorBrush(COLOR_APPWORKSPACE) );
  setCollapseHeight(0);
  setExpanded(TRUE);
  oldw = 0;

  appendWindowStyle(WS_TABSTOP);
  appendWindowStyleEx(WS_EX_STATICEDGE);
}


wndForest::~wndForest() {
}


void wndForest::onInit() {
  wndForestBase::onInit();
  scrollwnd->init();
  scrollwnd->moveresize(0, 0, 0, 0);
  scrollwnd->showWindow();
}


void wndForest::onPostInit() {
  wndForestBase::onPostInit();
  resizeObjects();
}

void wndForest::onResize(int width, int height) {
  wndForestBase::onResize(width, height);

  updateScroll();
  onVertScroll(getVertScrollPos());


  scrollwnd->setWidth(getClientWidth(), FALSE);
  if (width != oldw) {
    scrollwnd->invalidate();
    adjustWindowWidths(TRUE);
    scrollwnd->updateWindow();
  }

  oldw = width;


  int ih = 0;
  wndForestBase *o = getFirstForestChild();
  while (o) {
    if (o->isVisible()) {
      ih += o->getWindowHeight();
    }
    o = getNextForestChild(o);
  }

  RECT r;
  getWindowRect(&r);
  r.top += ih;
//  invalidate(&r);
  RedrawWindow(getWnd(), NULL, NULL, RDW_FRAME|RDW_INVALIDATE);
}


void wndForest::onVertScroll(int pos) {
  whkScroll::onVertScroll(pos);
  int oldy = scrollwnd->getY();
  int newy = -pos;
  scrollwnd->setY(newy);
}


void wndForest::onPaint(HDC hdc, PAINTSTRUCT *ps) {
  int ih = 0;
  wndForestBase *o = getFirstForestChild();
  while (o) {
    if (o->isVisible()) {
      ih += o->getWindowHeight();
    }
    o = getNextForestChild(o);
  }

  RECT r;
  getClientRect(&r);
  r.top = ih;

  FillRect(hdc, &r, (HBRUSH)getBackgroundBrush());
}


BOOL wndForest::eraseBackground(HDC hdc) {
  return TRUE;
}



void wndForest::onChildSized(wndForestBase *object) {
  wndForestBase::onChildSized(object);
  updateScroll();
//  invalidate();
}


int wndForest::updateScroll() {
//  if (!isPostInited()) return 0;
  scrollwnd->setHeight(getExpandedClientHeight());

  int ret = 0;
  if ( getExpandedClientHeight() > getClientHeight() ) {
    if (!getVertScroll()) {
      ret = 1;
      setVertScroll(TRUE);
      scrollwnd->setWidth(getClientWidth());
      scrollwnd->invalidate();
      adjustWindowWidths();
    }
    setVertScrollMinMax(0, getExpandedClientHeight()-1);
    setVertScrollPage(getClientHeight());
    
    int scrollmax = getVertScrollMax()-getVertScrollPage()+1;
    if (getVertScrollPos() >= scrollmax) onVertScroll(scrollmax);

  } else {
    if (getVertScroll()) {
      ret = 2;
      setVertScrollPos(0);
      onVertScroll(0);
      setVertScroll(FALSE);
      scrollwnd->setWidth(getClientWidth());
      scrollwnd->invalidate();
      adjustWindowWidths();
    }
  }
  return ret;
}


VUINT wndForest::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == WM_MOUSEWHEEL) {
    if (getVertScroll()) {
      short delta = (short)HIWORD(param1);
      int pos = getVertScrollPos();
      if (delta > 0) {
        pos -= getVertScrollPage()>>2;
      } else {
        pos += getVertScrollPage()>>2;
      }
      pos = max(pos, getVertScrollMin());
      pos = min(pos, getVertScrollMax()-getVertScrollPage()+1);
      onVertScroll(pos);
    }
    return 0;
  }
  return wndForestBase::wndProc(msg, param1, param2);
}


#endif
