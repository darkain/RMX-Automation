/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CONS_BASE_H_
#define _CONS_BASE_H_


#include "../cfg/cfgBase.h"


//TODO: add params for if this accepts broadcasts and global broadcasts or not


class consBase : public cfgBase {
  public:
    consBase(const char *name, cfgBase *parent=NULL, HPARAMLIST paramlist=NULL);
    virtual ~consBase();

  protected:
    virtual void print(const char *s) {}
    virtual void clear() {}

  public:
    void grabActiveHandle();
    void releaseActiveHandle();

  protected:
    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2);
};


#endif //_CONS_BASE_H_
