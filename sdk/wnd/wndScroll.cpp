/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "../rmxBase.h"
#include "wndScroll.h"



/*
vScrollWnd::vScrollWnd(wndBase *parent) : wndBase("ScrollParent", parent) {
//  setCreateStyle(WND_REGISTERCLASS);
  setName("", FALSE);
  scrollbar = NULL;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
}


vScrollWnd::~vScrollWnd() {
}


void vScrollWnd::onInit() {
  wndBase::onInit();
  scrollbar = new wndBase("ScrollBar", this);
//  scrollbar->setDrawStyle(scrollbar->getDrawStyle() | SBS_VERT);
  scrollbar->appendWindowStyle(SBS_VERT);
  scrollbar->init();
  scrollbar->showWindow();
  setMinMax(0,0);
  setPageSize(1);
  setScrollPosition(0);
}


void vScrollWnd::scrollUp(UINT amt) {
  setScrollPosition(getScrollPosition() - amt);
}

void vScrollWnd::scrollDown(UINT amt) {
  setScrollPosition(getScrollPosition() + amt);
}

void vScrollWnd::scrollPageUp(UINT amt) {
  setScrollPosition(getScrollPosition() - (amt * getPageSize()));
}

void vScrollWnd::scrollPageDown(UINT amt) {
  setScrollPosition(getScrollPosition() + (amt * getPageSize()));
}

UINT vScrollWnd::getPageSize() {
  info.fMask = SIF_PAGE;
  GetScrollInfo(scrollbar->getWnd(), SB_CTL, &info);
  return info.nPage;
}

void vScrollWnd::setPageSize(UINT size) {
  info.fMask = SIF_PAGE;
  info.nPage = size;
  SetScrollInfo(scrollbar->getWnd(), SB_CTL, &info, TRUE);
}

void vScrollWnd::setMinMax(int min, int max) {
  info.fMask = SIF_RANGE;
  info.nMin = min;
  info.nMax = max;
  SetScrollInfo(scrollbar->getWnd(), SB_CTL, &info, TRUE);
}

void vScrollWnd::setMin(int min) {
  setMinMax(min, getMax());
}

void vScrollWnd::setMax(int max) {
  setMinMax(getMin(), max);
}

int vScrollWnd::getMin() {
  info.fMask = SIF_RANGE;
  GetScrollInfo(scrollbar->getWnd(), SB_CTL, &info);
  return info.nMin;
}

int vScrollWnd::getMax() {
  info.fMask = SIF_RANGE;
  GetScrollInfo(scrollbar->getWnd(), SB_CTL, &info);
  return info.nMax;
}

void vScrollWnd::scrollToMin() {
  setScrollPosition(getMin());
}

void vScrollWnd::scrollToMax() {
  setScrollPosition(getMax());
}


void vScrollWnd::setScrollPosition(int pos, BOOL redraw) {
  int max = getMax() - getPageSize() + getMin();
  if (pos > max) pos = max;
  if (pos < getMin()) pos = getMin();
  scrollbar->sendMessage(SBM_SETPOS, pos, redraw);
  onScroll(pos);
}


int vScrollWnd::getScrollPosition() {
  return scrollbar->sendMessage(SBM_GETPOS);
}


void vScrollWnd::onResize(int width, int height) {
  wndBase::onResize(width, height);
  if (scrollbar) scrollbar->moveresize(0, 0, getClientWidth(), getClientHeight());
}


UINT vScrollWnd::wndProc(UINT msg, UINT param1, UINT param2) {
  switch (msg) {
    case WM_VSCROLL:
    case WM_HSCROLL:
      switch (LOWORD(param1)) {
        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
          setScrollPosition(HIWORD(param1), FALSE);
          break;

        case SB_LINEDOWN: scrollDown(); break;
        case SB_LINEUP: scrollUp(); break;
        case SB_PAGEDOWN: scrollPageDown(); break;
        case SB_PAGEUP: scrollPageUp(); break;
        case SB_BOTTOM: scrollToMax(); break;
        case SB_TOP: scrollToMin(); break;

        case SB_ENDSCROLL: onScrollEnd(); break;
      }
      break;
  }
  return wndBase::wndProc(msg, param1, param2);
}

*/


#endif
