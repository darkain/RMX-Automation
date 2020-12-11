/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __CORE_TCP_HTTP_H__
#define __CORE_TCP_HTTP_H__


#include "../../../libraries/jnetlib/webserver.h"
#include "../../../sdk/Win32/Timer.h"


/*
class MemPageGenerator : public IPageGenerator, public rmxString {
public:
	MemPageGenerator(const char *string) : rmxString(string) { position = 0; }
	MemPageGenerator(rmxString  &string) : rmxString(string) { position = 0; }
	
	virtual ~MemPageGenerator() {}

public:
	virtual int GetData(char *buffer, int size) { // return 0 when done
		if (size > length() - position) size = length() - position;
		const char *data = *this;
		memcpy(buffer, data+position, size);
		position += size;
		return size;
	}

private:
	int position;
};
*/



class rmx_server : public WebServerBaseClass, public vTimer {
public:
	rmx_server() : vTimer(10, true) { addListenPort(31080); } 
	virtual ~rmx_server() {}

public:
    virtual void onTimer(DWORD systemTime) { run(); }

	virtual IPageGenerator *onConnection(JNL_HTTPServ *serv, int port);
};


#endif //__CORE_TCP_HTTP_H__
