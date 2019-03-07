/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cbBase.h"
#include "../cfg/cfgBase.h"


cbBase::cbBase(cfgBase *item) {
	if (item) appendItem(item);
}


cbBase::~cbBase() {
	emptyItems();
}


void cbBase::onInsertItem(cfgBase *data, cfgBase *before) {
	data->insertCallback(this);
}


void cbBase::onDelete(cfgBase *data) {
	data->removeCallback(this);
}


void cbBase::cb_onDelete(cfgBase *item) {
	removeItem(item);
	if (!hasItems()) delete this;
}
