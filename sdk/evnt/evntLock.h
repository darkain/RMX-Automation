/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _EVNT_LOCK_H_
#define _EVNT_LOCK_H_


//TODO:  check to see if this is deprecated


#include "evntSingle.h"
#include "../rmxLock.h"


class evntLock : public evntSingle, public rmxLock {
  public:
    evntLock(const char *initname, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~evntLock() {}

  public:
    virtual void activate();

  protected:
    virtual void onActivated();
    virtual void onTimer(DWORD systemTime);

  private:
    int count;
};


#endif //_EVNT_LOCK_H_
