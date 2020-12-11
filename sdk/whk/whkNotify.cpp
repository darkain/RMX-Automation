/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "whkNotify.h"


whkNotify::whkNotify(wndBase *parentwnd) : whkBase(parentwnd) {
}


BOOL whkNotify::wndHookProc(whkInfo *info) {
  if (info) if (info->msg == WM_NOTIFY) {
    NMHDR *notify = (NMHDR*)info->param2;
    return onNotify(info, notify);
  }
  return whkBase::wndHookProc(info);
}
