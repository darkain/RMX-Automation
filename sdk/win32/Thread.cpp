/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "Thread.h"


DWORD WINAPI vThreadProc(void *pThis) {
  if (pThis) {
    vThread *t = (vThread*)pThis;
    DWORD del = t->ThreadProc();
    if (del) delete t;
    return del;
  }
  return 0;
}


vThread::vThread(BOOL selfDeleting) {
  thread = NULL;
  id = 0;
  running = false;
  selfDel = selfDeleting;
  selfDelNow = FALSE;
  mutex = NULL;
}



vThread::~vThread() {
  End();
}


void vThread::Begin(int priority) {
  if (thread) {
    SetThreadPriority(thread, priority);
    return;
  }

  mutex = CreateMutex(NULL, FALSE, NULL);
  running = TRUE;
  thread = CreateThread(NULL, 65536, (LPTHREAD_START_ROUTINE)vThreadProc, this, 0, (LPDWORD)&id);

  if (!thread) {
    running = FALSE;
    return;
  }

  SetThreadPriority(thread, priority);
  onBegin();
}


void vThread::End() {
  beginThreadSafe();
  running = FALSE;
  endThreadSafe();

  if (thread) {
    WaitForSingleObject(thread, 2000);
    CloseHandle(thread);
    CloseHandle(mutex);
    thread = NULL;
    mutex = NULL;
    onEnd();
  }
}


BOOL vThread::isRunning() volatile {
  beginThreadSafe();
  BOOL ret = running;
  endThreadSafe();
  return ret;
}


void vThread::beginThreadSafe() volatile {
  WaitForSingleObject(mutex, INFINITE);
}


void vThread::endThreadSafe() volatile {
  ReleaseMutex(mutex);
}
