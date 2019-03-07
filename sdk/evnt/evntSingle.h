/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _EVNT_SINGLE_H_
#define _EVNT_SINGLE_H_


#include "evntPress.h"
#include "../win32/timer.h"


class evntSingle : public evntPress, public vTimer {
  public:
    evntSingle(const char *initname, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~evntSingle() {}

  protected:
    virtual void onActivated();
    virtual void onTimer(DWORD systemTime);

  public:
    virtual void activate();
};


#endif //_EVNT_SINGLE_H_
