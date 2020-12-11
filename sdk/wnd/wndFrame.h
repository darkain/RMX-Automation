/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_FRAME_H_
#define _WND_FRAME_H_


#include "wndBase.h"


class wndFrame : public wndBase {
  public:
    wndFrame(wndBase *parent=NULL);
    virtual ~wndFrame();
};


#endif //_WND_FRAME_H_
