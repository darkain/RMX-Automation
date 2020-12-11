/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __THREAD_H_
#define __THREAD_H_


#include "Win32.h"


class vThread {
  protected:
    vThread(BOOL selfDeleting=false);
  public:
    virtual ~vThread();

  public:
    void Begin(int priority=THREAD_PRIORITY_IDLE);
    virtual void onBegin() {}

    void End();
    virtual void onEnd() {}

    BOOL isRunning() volatile;
    virtual DWORD ThreadProc() { selfDelNow=TRUE; return selfDel; }

    inline void setDeleteNow(BOOL delNow) { selfDelNow = delNow; }
    inline BOOL getDeleteNow()  const { return selfDelNow; }
    inline BOOL getSelfDelete() const { return selfDel; }

    inline HANDLE getThread() const { return thread; }

    virtual void onPriorityChange() {}
    inline void setThreadPriority(int p) { if (thread) { SetThreadPriority(thread, p); onPriorityChange(); } }
    inline int getThreadPriority() const { return GetThreadPriority(thread); }


    void beginThreadSafe() volatile;
    void endThreadSafe()   volatile;

  private:
    HANDLE  thread;
    DWORD   id;
    BOOL    selfDel;
    BOOL    selfDelNow;

    volatile BOOL running;
    HANDLE mutex;
};


#endif __THREAD_H_
