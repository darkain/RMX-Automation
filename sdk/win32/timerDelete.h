/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _TIMER_DELETE_H_
#define _TIMER_DELETE_H_


#include "Timer.h"
#include "../rmxName.h"


class timerDelete : public vTimedCallback {
  public:
    timerDelete() {}
    virtual ~timerDelete() {}

  public:
    void deleteThis() {
      addEvent(this);
    }

  protected:
    virtual void onEvent(void *event) {
      if (event==(void*)this) delete this;
    }
};


//TODO OBSOLETE THIS CLASS, IT IS BAAAAD - possible race conditions
class timerString : public timerDelete, public rmxString {
  public:
    timerString(const char *initialName=NULL) : rmxString(initialName) {}
    virtual ~timerString() {}
};


#endif //_TIMER_DELETE_H_
