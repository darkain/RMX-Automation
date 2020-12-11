/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __CORE_TCP_BASE_H__
#define __CORE_TCP_BASE_H__


#include "../../../libraries/jnetlib/webserver.h"
#include "../../../sdk/rmxString.h"


class httpBase : public IPageGenerator, public rmxString {
public:
	httpBase(JNL_HTTPServ *httpserver);
	virtual ~httpBase();

	using rmxString::operator =;

public:
	inline JNL_HTTPServ *getServer() const { return server; }


protected:
	void setBuffer(const char *data);
	virtual int GetData(char *buffer, int size);

protected:
	JNL_HTTPServ *server;
	int position;
};


#endif //__CORE_TCP_BASE_H__
