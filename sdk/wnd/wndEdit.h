/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_EDIT_H_
#define _WND_EDIT_H_


#include "wndSubclass.h"


class wndEdit : public wndSubclass {
  public:
    wndEdit(wndBase *parent);
    virtual ~wndEdit() {}

  protected:
    virtual VUINT subWndProc(UINT msg, VUINT param1, VUINT param2);
};


#endif //_WND_EDIT_H_
