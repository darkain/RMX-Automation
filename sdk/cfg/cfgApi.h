/******************************  RMX SDK  ******************************\
*  Copyright (c) 2012 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_API_H_
#define _CFG_API_H_


#include "cfgBase.h"


class cfgApi : public cfgBase {
public:
	cfgApi(const char *name, const char *urlpath)
		: cfgBase(name, "rmxapi") {
		setParam("urlpath", urlpath);
	}

	virtual ~cfgApi() {
	}

public:
	inline const char *getUrlPath() const { getParam("urlpath"); }
};


#endif //_CFG_API_H_
