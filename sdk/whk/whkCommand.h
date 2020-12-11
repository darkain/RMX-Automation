/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WHK_COMMAND_H_
#define _WHK_COMMAND_H_


#include "whkBase.h"


class whkCommand : public whkBase {
  public:
    whkCommand(wndBase *parentwnd=NULL);
    virtual ~whkCommand() {}

  protected:
    virtual BOOL onCommand(whkInfo *info) { return FALSE; }
    virtual BOOL wndHookProc(whkInfo *info);
};


#endif //_COMMANDWNDHOOK_H_
