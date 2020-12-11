/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "prmBase.h"
#include "../cfg/cfgBase.h"


void prmBase::onDataUpdated() {
	if (item) item->dispatchMessage(CB_PARAM_UPDATED, this);
}


void prmBase::setParamConfig(cfgBase *config) {
	item = config;
	if (item) {
		setSecurity(item->getSecurity() & 0x0F);
		item->params.appendItem(this);
		item->dispatchMessage(CB_PARAM_INSERT, this);
	}
}
