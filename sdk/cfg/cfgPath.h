/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_PATH_H_
#define _CFG_PATH_H_


#include "cfgBase.h"


/*
this class uses UserData to store its displayed path information
i *might* combine this class w/ Text, and make it use a "style" attrib
*/


class cfgPath : public cfgBase {
public:
	cfgPath(const char *name, cfgBase *par=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
	virtual ~cfgPath() {}

public:
	const char *getPath()             const { return getValue();           }
	void setPath(const char *path)          { setValueString(path);        }
	const char *getPathStyle()        const { return getParam("style");    }
	void setPathStyle(const char *pathtype) { setParam("style", pathtype); }
};


#endif //_CFG_PATH_H_
