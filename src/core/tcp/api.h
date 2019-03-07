/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __CORE_TCP_API_H__
#define __CORE_TCP_API_H__


#include "base.h"


class rmxDataList;


class httpApi : public httpBase {
public:
	httpApi(JNL_HTTPServ *httpserver, rmxDataList *url_list);
	virtual ~httpApi();

protected:
	rmxDataList  *list;
};


#endif //__CORE_TCP_API_H__
