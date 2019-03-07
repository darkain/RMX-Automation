/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __CORE_TCP_API_CORE_H__
#define __CORE_TCP_API_CORE_H__


#include "base.h"


class rmxDataList;
class vPlugin;
class rmxString;


class httpApiCore : public httpBase {
public:
	httpApiCore(JNL_HTTPServ *httpserver, rmxDataList *url_list);
	virtual ~httpApiCore();

protected:
	void dumpList(vPlugin *plugin, HLIST list, rmxString &output, bool last=false);

protected:
	rmxDataList  *list;
};


#endif //__CORE_TCP_API_CORE_H__
