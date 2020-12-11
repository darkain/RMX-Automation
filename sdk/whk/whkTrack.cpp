/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "whkTrack.h"
#include <commctrl.h>


whkTrack::whkTrack(wndBase *parentwnd) : whkBase(parentwnd) {
}

whkTrack::~whkTrack() {
}


int whkTrack::getTrackWndPos() {
  return (int)SendMessage(getHookWnd(), TBM_GETPOS, 0, 0);
}

int whkTrack::getTrackWndMin() {
  return 0;
}

int whkTrack::getTrackWndMax() {
  return 0;
}

int whkTrack::getTrackWndPage() {
  return 0;
}

void whkTrack::setTrackWndPos(int pos) {
  SendMessage(getHookWnd(), TBM_SETPOS, TRUE, pos);
}

void whkTrack::setTrackWndMin(int min) {
  SendMessage(getHookWnd(), TBM_SETRANGEMIN, TRUE, min);
}

void whkTrack::setTrackWndMax(int max) {
  SendMessage(getHookWnd(), TBM_SETRANGEMAX, TRUE, max);
}

void whkTrack::setTrackWndMinMan(int min, int max) {
  setTrackWndMin(min);
  setTrackWndMax(max);
}

void whkTrack::setTrackWndPage(int page) {
}


BOOL whkTrack::wndHookProc(whkInfo *info) {
  if (!info) return whkBase::wndHookProc(info);

  if (info->msg == WM_HSCROLL) {
    switch (LOWORD(info->param1)) {
      case SB_ENDSCROLL:
      case SB_TOP:
      case SB_BOTTOM:
      case SB_LINEUP:
      case SB_LINEDOWN:
      case SB_PAGEUP:
      case SB_PAGEDOWN:
      case SB_THUMBTRACK:    //do the same as the next line
      case SB_THUMBPOSITION:
        //onTrackWnd((short)HIWORD(info->param1));
        onTrackWnd(getTrackWndPos());
        return 0;
    }
    return TRUE;
  }
  return whkBase::wndHookProc(info);
}
