/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _DX_DRAWWND_H_
#define _DX_DRAWWND_H_


#include "vDirectX.h"
#include "dxRender.h"
#include "../wnd/wndBase.h"
#include "../cfg/cfgBase.h"



class dxWnd : public wndBase, public cfgBase, public vDirectX {
  public:
    dxWnd(const char *name, cfgBase *parent);
    virtual ~dxWnd();
  
  protected:
    virtual void onPostInit();

    virtual void onPreRender() {}
    virtual void onPostRender() {}
  
  
  public:

    void render();

    inline void setName(const char *name) { wndBase::setName(name);  cfgBase::setName(name); }

  private:
    static rmxList<dxWnd*> dxwnds;
};


#endif //_DX_DRAWWND_H_
