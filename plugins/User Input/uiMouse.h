/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _UI_MOUSE_H_
#define _UI_MOUSE_H_


#include "../../sdk/rmxLock.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/func/funcAbsolute.h"


class uiMouse : public cfgTree, public rmxLock {
  public:
    uiMouse(cfgBase *parent);
    virtual ~uiMouse();

  public:
    HHOOK getHook() const { return hook; }

    inline int getX() const { return x->getParamInt("value"); }
    inline int getY() const { return y->getParamInt("value"); }
  
  private:
    void setMouseCoords(int xpos, int ypos);
    static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);

  private:
    HHOOK hook;
    funcAbsolute *x;
    funcAbsolute *y;
};


extern uiMouse *mouse;


#endif //_UI_MOUSE_H_
