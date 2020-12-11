/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "rmxBase.h"
#include "rmxMutex.h"
#include "rmxOS.h"


rmxMutex::rmxMutex(bool locked) {
#if defined _RMX_WIN32_ || defined _RMX_WIN64_
	mutex = (__int64) CreateMutex(NULL, FALSE, NULL);
#elif defined _RMX_LINUX_
	mutex = (__int64) PTHREAD_MUTEX_INITIALIZER;
#else
#error PORTME
#endif

	slock = locked;
}


rmxMutex::~rmxMutex() {
#if defined _RMX_WIN32_ || defined _RMX_WIN64_
	CloseHandle((HANDLE)mutex);
#endif

	mutex = 0;
}


bool rmxMutex::lock() volatile {
#if defined _RMX_WIN32_ || defined _RMX_WIN64_
	int value = WaitForSingleObject((HANDLE)mutex, 120*1000);
	if (value != WAIT_OBJECT_0) return false;
#elif defined _RMX_LINUX_
	pthread_mutex_lock(&mutex);
#else
#error PORTME
#endif 
	slock = true;
	return true;
}


bool rmxMutex::unlock() volatile {
#if defined _RMX_WIN32_ || defined _RMX_WIN64_
	int value = ReleaseMutex((HANDLE)mutex);
	if (value == 0) return false;
#elif defined _RMX_LINUX_
	pthread_mutex_unlock(&mutex);
#else
#error PORTME
#endif 
	slock = false;
	return true;
}
