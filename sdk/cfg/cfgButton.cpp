/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cfgButton.h"


cfgButton::cfgButton(const char *name, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : cfgBase(name, "button", parent, insert, paramlist) {

	setParam("text", "")->setSavable(false);
	setParam("desc", "")->setSavable(false);
}


cfgButton::cfgButton(const char *name, const char *text, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : cfgBase(name, "button", parent, insert, paramlist) {

	setParam("text", "")->setSavable(false);
	setParam("desc", "")->setSavable(false);
}


cfgButton::cfgButton(const char *name, const char *text, const char *desc, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : cfgBase(name, "button", parent, insert, paramlist) {

	setParam("text", "")->setSavable(false);
	setParam("desc", "")->setSavable(false);
}


cfgButton::~cfgButton() {
}


RMXLONG cfgButton::onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
	if (isEnabled()) {
		if (message == GENERIC_CLICK) {
			onClick(source);
			return 0;
		}
	}
	return cfgBase::onMessage(source, message, param1, param2);
}