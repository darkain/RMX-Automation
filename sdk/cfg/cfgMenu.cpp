/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cfgMenu.h"


cfgMenu::cfgMenu(const char *initname, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : cfgBase(initname, "menu", parent, insert, paramlist) {

	setSavable(FALSE);
	setParamInt("checked", 0)->setSavable(false);
}


cfgMenu::~cfgMenu() {
}


RMXLONG cfgMenu::onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
	if (message == GENERIC_CLICK) onClick(source);
	return cfgBase::onMessage(source, message, param1, param2);
}


cfgMenuSep::cfgMenuSep(cfgBase *parent, cfgBase *insert) : cfgMenu("-", parent, insert) {
}
