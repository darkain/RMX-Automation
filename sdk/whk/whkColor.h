/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WHK_COLOR_H_
#define _WHK_COLOR_H_


#include "whkBase.h"


class whkColor : public whkBase {
  public:
    whkColor(wndBase *parentwnd=NULL);
    virtual ~whkColor() {}

  protected:
    virtual BOOL wndHookProc(whkInfo *info);
};


#endif //_WHK_COLOR_H_
