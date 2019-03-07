/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WHK_NOTIFY_H_
#define _WHK_NOTIFY_H_


#include "whkBase.h"


class whkNotify : public whkBase {
  public:
    whkNotify(wndBase *parentwnd=NULL);
    virtual ~whkNotify() {}

  protected:
    virtual BOOL onNotify(whkInfo *info, NMHDR *notify) { return FALSE; }
    virtual BOOL wndHookProc(whkInfo *info);
};


#endif //_WHK_NOTIFY_H_
