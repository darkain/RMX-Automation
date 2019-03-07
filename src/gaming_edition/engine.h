/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _ENGINE_H_
#define _ENGINE_H_


#include "../../sdk/rmxBase.h"
#include "../rmx_win32/rmx_win32.h"


class engWnd;


class vEngine : public vRmxWin32 {
  public:
    vEngine();
    virtual ~vEngine();

  protected:
    virtual void initMasterPlugin(vLocalPlugin *master);

    virtual BOOL onRmxPostInit();

  private:
    engWnd *wnd;
};



extern vEngine *ENGINE;


#endif //_ENGINE_H_