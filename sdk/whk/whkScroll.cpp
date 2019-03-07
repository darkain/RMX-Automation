/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "whkScroll.h"


BOOL whkScroll::wndHookProc(whkInfo *info) {
  if (!info) return whkBase::wndHookProc(info);

  switch (info->msg) {
    case WM_HSCROLL:
      switch (LOWORD(info->param1)) {
        case SB_THUMBTRACK:    //do the same as the next line
        case SB_THUMBPOSITION: onHorzScroll((short)HIWORD(info->param1)); break;
        case SB_TOP:      onHorzScroll(getHorzScrollMin()); break;
        case SB_BOTTOM:   onHorzScroll(getHorzScrollMax()); break;
        case SB_LINEUP:   onHorzScroll( max( getHorzScrollPos()-20, getHorzScrollMin() ) ); break;
        case SB_LINEDOWN: onHorzScroll( min( getHorzScrollPos()+20, getHorzScrollMax()-getHorzScrollPage()+1 ) ); break;
        case SB_PAGEUP:   onHorzScroll( max( getHorzScrollPos()-getHorzScrollPage(), getHorzScrollMin() ) ); break;
        case SB_PAGEDOWN: onHorzScroll( min( getHorzScrollPos()+getHorzScrollPage(), getHorzScrollMax()-getHorzScrollPage()+1 ) ); break;
      }
      break;

    case WM_VSCROLL:
      switch (LOWORD(info->param1)) {
        case SB_THUMBTRACK:    //do the same as the next line
        case SB_THUMBPOSITION: onVertScroll(HIWORD(info->param1)); break;
        case SB_TOP:      onVertScroll(getVertScrollMin()); break;
        case SB_BOTTOM:   onVertScroll(getVertScrollMax()); break;
        case SB_LINEUP:   onVertScroll( max( getVertScrollPos()-20, getVertScrollMin() ) ); break;
        case SB_LINEDOWN: onVertScroll( min( getVertScrollPos()+20, getVertScrollMax()-getVertScrollPage()+1 ) ); break;
        case SB_PAGEUP:   onVertScroll( max( getVertScrollPos()-getVertScrollPage(), getVertScrollMin() ) ); break;
        case SB_PAGEDOWN: onVertScroll( min( getVertScrollPos()+getVertScrollPage(), getVertScrollMax()-getVertScrollPage()+1 ) ); break;
      }
      break;

    default:
      return whkBase::wndHookProc(info);
  }

  return TRUE;
}


BOOL whkScroll::getHorzScroll() {
  wndBase *wnd = getFirstHookWnd();
  if (wnd) return !!(wnd->getWindowStyle() & WS_HSCROLL);
  return FALSE;
}

void whkScroll::setHorzScroll(BOOL visible) {
  ShowScrollBar(getHookWnd(), SB_HORZ, visible);
}

int whkScroll::getHorzScrollPos() {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_POS;
  GetScrollInfo(getHookWnd(), SB_HORZ, &info);
  return info.nPos;
}

int whkScroll::getHorzScrollMin() {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_RANGE;
  GetScrollInfo(getHookWnd(), SB_HORZ, &info);
  return info.nMin;
}

int whkScroll::getHorzScrollMax() {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_RANGE;
  GetScrollInfo(getHookWnd(), SB_HORZ, &info);
  return info.nMax;
}

int whkScroll::getHorzScrollPage() {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_PAGE;
  GetScrollInfo(getHookWnd(), SB_HORZ, &info);
  return info.nPage;
}

void whkScroll::setHorzScrollMinMax(int min, int max) {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_RANGE;
  info.nMin = min;
  info.nMax = max;
  SetScrollInfo(getHookWnd(), SB_HORZ, &info, TRUE);
}

void whkScroll::setHorzScrollPos(int pos) {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_POS;
  info.nPos = pos;
  SetScrollInfo(getHookWnd(), SB_HORZ, &info, TRUE);
}

void whkScroll::setHorzScrollPage(int page) {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_PAGE;
  info.nPage = page;
  SetScrollInfo(getHookWnd(), SB_HORZ, &info, TRUE);
}

void whkScroll::onHorzScroll(int pos) {
  setHorzScrollPos(pos);
}



BOOL whkScroll::getVertScroll() {
  wndBase *wnd = getFirstHookWnd();
  if (wnd) return !!(wnd->getWindowStyle() & WS_VSCROLL);
  return FALSE;
}

void whkScroll::setVertScroll(BOOL visible) {
  ShowScrollBar(getHookWnd(), SB_VERT, visible);
}

int whkScroll::getVertScrollPos() {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_POS;
  GetScrollInfo(getHookWnd(), SB_VERT, &info);
  return info.nPos;
}

int whkScroll::getVertScrollMin() {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_RANGE;
  GetScrollInfo(getHookWnd(), SB_VERT, &info);
  return info.nMin;
}

int whkScroll::getVertScrollMax() {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_RANGE;
  GetScrollInfo(getHookWnd(), SB_VERT, &info);
  return info.nMax;
}

int whkScroll::getVertScrollPage() {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_PAGE;
  GetScrollInfo(getHookWnd(), SB_VERT, &info);
  return info.nPage;
}

void whkScroll::setVertScrollMinMax(int min, int max) {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_RANGE;
  info.nMin = min;
  info.nMax = max;
  SetScrollInfo(getHookWnd(), SB_VERT, &info, TRUE);
}

void whkScroll::setVertScrollPos(int pos) {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_POS;
  info.nPos = pos;
  SetScrollInfo(getHookWnd(), SB_VERT, &info, TRUE);
}

void whkScroll::setVertScrollPage(int page) {
  SCROLLINFO info;
  ZeroMemory(&info, sizeof(SCROLLINFO));
  info.cbSize = sizeof(SCROLLINFO);
  info.fMask = SIF_PAGE;
  info.nPage = page;
  SetScrollInfo(getHookWnd(), SB_VERT, &info, TRUE);
}

void whkScroll::onVertScroll(int pos) {
  setVertScrollPos(pos);
}
