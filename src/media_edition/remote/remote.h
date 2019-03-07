/******************************  RMX SDK  ******************************\
*  Copyright (c) 2010 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_REMOTE_H_
#define _RMX_REMOTE_H_


#include "../../../sdk/Win32/Win32.h"
#include "../../../sdk/Win32/Thread.h"
#include "../../../sdk/Win32/Timer.h"
#include "../../rmx_win32/rmx_win32.h"


class rmxUdpRemote : public vThread, public vTimer {
	public:
		rmxUdpRemote();
		~rmxUdpRemote();

	protected:
		virtual DWORD ThreadProc();
		virtual void onTimer(DWORD systemTime);

	private:
		rmxDeleteList<rmxString*> list;
};


#endif //_RMX_REMOTE_H_
