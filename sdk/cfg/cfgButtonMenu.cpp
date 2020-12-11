/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cfgButtonMenu.h"


cfgButtonMenu::cfgButtonMenu(const char *initname, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : cfgButton(initname, parent, insert, paramlist) {
}


cfgButtonMenu::~cfgButtonMenu() {
}


void cfgButtonMenu::onClick(HPLUGIN source) {
  source->list_message(source->list_getRoot(), MENU_POP, (RMXLONG)getListHandle());
}
