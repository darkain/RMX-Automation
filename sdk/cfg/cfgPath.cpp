/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cfgPath.h"


cfgPath::cfgPath(const char *name, cfgBase *par, cfgBase *insert, HPARAMLIST paramlist)
 : cfgBase(name, "path", par, insert, paramlist) {

	setParam("style", "")->setSavable(false);
}
