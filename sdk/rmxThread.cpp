/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/



#include "rmxBase.h"
#include "rmxThread.h"
#include "rmxOS.h"


#ifdef _WIN32
DWORD WINAPI rmxThreadProc(void *p) {
#else // Unix, Linux, Posix
unsigned int rmxThreadProc(void *p) {
#endif
	if (!p) return 0;

	rmxThread *t = (rmxThread*)p;
	int del = t->threadProc();
	if (del) delete t;
	return del;
}


rmxThread::rmxThread(bool selfDeleting) {
	thread      = NULL;
	id          = 0;
	running     = false;
	selfDel     = selfDeleting;
	selfDelNow  = false;
}



rmxThread::~rmxThread() {
	End();
}


void rmxThread::Begin(int priority) {
	if (thread) {
		setThreadPriority(priority);
		return;
	}

	running = false;
	onThreadPreBegin();
	running = true;

#ifdef _WIN32
	thread = (__int64) CreateThread(NULL, 0x0000FFFF, (LPTHREAD_START_ROUTINE)rmxThreadProc, this, 0, (LPDWORD)&id);
#else // Unix, Linux, Posix
	pthread_create(&thread, NULL, (void*(*)(void*))rmxThreadProc, (void*)this);
#endif

	if (!thread) {
		running = false;
		return;
	}

	setThreadPriority(priority);
	onThreadPostBegin();
}


void rmxThread::End() {
	onThreadPreEnd();

	lock();
	running = false;
	unlock();

	if (thread) {
#ifdef _WIN32
		WaitForSingleObject((HANDLE)thread, 2000);
		CloseHandle((HANDLE)thread);
		thread = NULL;
		onThreadPostEnd();
#else
#error PORTME
#endif
	}
}


bool rmxThread::isRunning() volatile {
	lock();
	bool ret = running;
	unlock();
	return ret;
}


void rmxThread::setThreadPriority(int p) {
	if (thread) {
#ifdef _WIN32
		SetThreadPriority((HANDLE)thread, p); onPriorityChange();
#else
#error PORTME
#endif
	}
}


int rmxThread::getThreadPriority() const {
#ifdef _WIN32
	return GetThreadPriority((HANDLE)thread);
#else
#error PORTME
#endif
}
