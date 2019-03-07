/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_MUTEX_H_
#define _RMX_MUTEX_H_


class rmxMutex {
public:
	rmxMutex(bool locked=false);
	virtual ~rmxMutex();

public:
	bool lock()   volatile;
	bool unlock() volatile;

	inline bool isLocked()   volatile { return  slock;  }
	inline bool isUnlocked() volatile { return !slock;  }

	inline void setLocked(bool locked) volatile { if (locked) lock(); else unlock(); }

private:
	volatile   bool  slock;
	volatile __int64 mutex;
};


#endif //_RMX_LOCK_H_
