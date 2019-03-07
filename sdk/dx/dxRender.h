/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _DX_RENDER_H_
#define _DX_RENDER_H_


#include "vDirectX.h"
#include "../cfg/cfgBase.h"


class dxRender : public cfgBase, public vDirectX {
  protected:
    dxRender(const char *name, const char *type, cfgBase *parent, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL) 
      : cfgBase(name, type, parent, insert, paramlist) {}

    ~dxRender() {}

  protected:
    //return the number of objects rendered
    virtual int onRender()=0;


    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
      if (message == GENERIC_PAINT) return onRender();
      return cfgBase::onMessage(source, message, param1, param2);
    }
};


#endif //_DX_RENDER_H_
