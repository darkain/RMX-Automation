/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "whkCommand.h"


whkCommand::whkCommand(wndBase *parentwnd) : whkBase(parentwnd) {
}


BOOL whkCommand::wndHookProc(whkInfo *info) {
  if (info  &&  info->msg == WM_COMMAND) {
    return onCommand(info);
  }
  return whkBase::wndHookProc(info);
}
