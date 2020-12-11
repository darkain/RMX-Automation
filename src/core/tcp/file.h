/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __CORE_TCP_FILE_H__
#define __CORE_TCP_FILE_H__


#include "../../../libraries/jnetlib/webserver.h"


class rmxDataList;


class httpFile : public IPageGenerator {
public:
	httpFile(JNL_HTTPServ *httpserver, rmxDataList *url_list);
	virtual ~httpFile();

public:
	inline JNL_HTTPServ *getServer() const { return server; }
	inline FILE *getFile() const { return file; }

protected:
	virtual int GetData(char *buf, int size);

protected:
	JNL_HTTPServ *server;
	rmxDataList  *list;
	FILE         *file;
};


#endif //__CORE_TCP_FILE_H__
