/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "funcGroup.h"
#include "../cfg/cfgRoot.h"


funcGroup::funcGroup(const char *function_name, const char *script_name, cfgRoot *parent, HPARAMLIST paramlist)
 : cfgBase(function_name, "group", parent, NULL, paramlist) {

	setParam("script", script_name)->setSavable(false);
}


funcGroup::funcGroup(const char *function_name, const char *script_name, funcGroup *parent, HPARAMLIST paramlist) 
 : cfgBase(function_name, "group", parent, NULL, paramlist) {

	setParam("script", script_name)->setSavable(false);
}


funcGroup::~funcGroup() {
}


const char *funcGroup::script_getValueSafe(const char *name, const char *safe) const {
	const char *ret = script_getValueString(name);
	return ((ret) ? (ret) : (safe));
}
