/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cfgNull.h"


cfgNull::cfgNull(const char *name, cfgBase *par, cfgBase *insert, HPARAMLIST paramlist)
 : cfgBase(name, "null", par, insert, paramlist) {

	setParam("align", "left")->setSavable(false);
}


cfgNull::cfgNull(const char *name, const char *type, cfgBase *par, cfgBase *insert, HPARAMLIST paramlist)
 : cfgBase(name, type, par, insert, paramlist) {

	setParam("align", "left")->setSavable(false);
}



cfgNull::~cfgNull() {
}


void cfgNull::validateParam(prmBase *param) {
	if (param->isNamed("align")) {
		if (param->isValued("left")) {}
		else if (param->isValued("center")) {}
		else if (param->isValued("right")) {}
		else {
			param->setValue("left");
		}
	}
	cfgBase::validateParam(param);
}
