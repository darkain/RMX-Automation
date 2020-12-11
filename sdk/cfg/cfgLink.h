/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_LINK_H_
#define _CFG_LINK_H_


#include "cfgNull.h"


/*
this class uses userdata as it's URL for the link
*/


class cfgLink : public cfgNull {
public:
	cfgLink(const char *itemName, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
	virtual ~cfgLink();

public:
	inline const char *getUrl() const { return getValue(); }
	inline cfgLink* setUrl(const char *url) { setValueString(url); return this; }
};


#endif //_CFG_LINK_H_
