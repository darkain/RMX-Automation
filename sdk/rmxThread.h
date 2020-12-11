/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_THREAD_H_
#define _RMX_THREAD_H_


#include "rmxMutex.h"


class rmxThread : public rmxMutex {
protected:
	rmxThread(bool selfDeleting=false);
public:
	virtual ~rmxThread();


protected:
	//NOTE: These are all called from the same thread
	//      which is calling Begin() or End()
	virtual void onThreadPreBegin()  {}
	virtual void onThreadPostBegin() {}
	virtual void onThreadPreEnd()    {}
	virtual void onThreadPostEnd()   {}

	virtual void onPriorityChange()  {}

public:
	virtual int threadProc() { selfDelNow=true; return selfDel; }
    

public:
	void Begin(int priority=0);  //TODO: create cross-platform thread priority IDs
	void End();

	bool isRunning() volatile;

	inline void setDeleteNow(bool delNow) { selfDelNow = delNow; }
	inline bool getDeleteNow()  const { return selfDelNow; }
	inline bool getSelfDelete() const { return selfDel; }

	inline __int64 getThread() const { return thread; }

	void setThreadPriority(int p);
	int getThreadPriority() const;


private:
	__int64 thread;
	__int64 id;

	bool    selfDel;
	bool    selfDelNow;

	volatile bool running;
};


#endif //_RMX_THREAD_H_
