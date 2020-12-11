/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_NULL_H_
#define _CFG_NULL_H_


#include "cfgBase.h"


/*
this class currently does not use UesrData, but that will soon change
when i work on some additional features for it to make it more usefull
*/


class cfgNull : public cfgBase {
  public:
    cfgNull(const char *name,                   cfgBase *par=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    cfgNull(const char *name, const char *type, cfgBase *par=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~cfgNull();

  protected:
    virtual void validateParam(prmBase *param);

  public:
    inline cfgNull* alignLeft()   { setParam("align", "left");   return this; }
    inline cfgNull* alignCenter() { setParam("align", "center"); return this; }
    inline cfgNull* alignRight()  { setParam("align", "right");  return this; }
};


#endif //_CFG_NULL_H_
