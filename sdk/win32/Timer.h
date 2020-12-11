/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __TIMERS_H_
#define __TIMERS_H_


#include "../rmxList.h"
#include "../Win32/MessageWnd.h"


class vTimerList;


//use this to create timers.
class vTimer {
  friend class vTimerList;

  protected:
    vTimer(UINT timeout=0, BOOL autoStart=FALSE);

  public:
    virtual ~vTimer();

  public:  //TODO: remove the systemTime variable
    virtual void onTimer(DWORD systemTime) {}

    void startTimer() volatile;
    void stopTimer() volatile;
    BOOL isStarted() volatile const { return Started; }

    void setTimeout(UINT ms);
    UINT getTimeout() const { return Timeout; }

    inline void resetTimeout() { setTimeout(getTimeout()); }

  protected:
    int Timeout;
    BOOL volatile Started;
    static vTimerList *list;
};


//a specific implementation of the timer...
//this is used to get delays on an event, this way
//you can process an event after some other peice
//of code executes... required in some areas do to
//the way the order of deleting happens to prevent
//crashing and other random buggies.
class vTimedCallback : public vTimer {
  protected:
    vTimedCallback();
    virtual ~vTimedCallback();

    virtual void onEvent(void *event) {}
    virtual void onTimer(DWORD systemTime);

  public:
    void addEvent(void *event);
    void removeEvent(void *event);

  private:
    rmxList<void*> events;
};


//TODO: This class is exploitable via race conditions!!! OBSOLETE ITS USAGE ASAP!!!
class vTimedDelete : public vTimer, public rmxString {
  public:
    vTimedDelete(const char *name) : vTimer(10, TRUE), rmxString(name) {}
    virtual ~vTimedDelete() {}

    virtual void onTimer(DWORD systemTime) { delete this; }
    static const char *create(const char *name) {
		vTimedDelete *t = new vTimedDelete(name);
		return t->get();
	}
};


//NOTE!!!!
//vTimeList now must be Init'ed
//InitTimers() will automatically be called by the plugin loader,
//so there is no need to worry about it, unless you are making
//something other than an RMX plugin w/ this SDK

//dont mess with this class directly at all!  let vTimer
//do all of the work for you
//this is for internal use only

class vTimerList : public rmxList<vTimer*>, public vMessageWnd {
  friend class vTimer;

  private:
    vTimerList();
    virtual ~vTimerList();
    void setTimer(vTimer *t);
    void killTimer(vTimer *t);

  protected:
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);
};


#endif  __TIMERS_H_