/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_LOCK_H_
#define _RMX_LOCK_H_


class rmxLock {
public:
	rmxLock(bool locked=false) { setLocked(locked); }
	~rmxLock() {}

public:
	inline void lock()                 { slock =   true;   }
	inline void unlock()               { slock =   false;  }
	inline bool isLocked()       const { return    slock;  }
	inline bool isUnlocked()     const { return   !slock;  }
	inline void setLocked(bool locked) { slock = !!locked; }

private:
	bool slock;
};


#endif //_RMX_LOCK_H_
