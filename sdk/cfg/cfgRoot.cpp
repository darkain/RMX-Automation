/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cfgRoot.h"


cfgRoot::cfgRoot(const char *name) : cfgBase(name, "root") {

#ifdef _DEBUG
#ifdef RMX_PLUGIN
	if (rootlist != NULL) {
		CONSOLE_MAIN->printError("SERIOUS FUCKING ERROR", "Multiple instances of class cfgRoot");
	}
#endif //RMX_PLUGIN
#endif //_DEBUG

	rootlist = this;

	//initial default config for the root list
	setSavable(TRUE);
	setReadOnly(FALSE);

	setParamInt("lib", _SDK_BUILD);  //version of RMX LIB (lib will overwrite this value)
	setParamInt("sdk", _SDK_BUILD);  //version of RMX SDK
}


cfgRoot::~cfgRoot() {
	rootlist = NULL;
}
