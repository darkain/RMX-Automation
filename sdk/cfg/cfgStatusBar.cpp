/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cfgStatusBar.h"


cfgStatusBar::cfgStatusBar(cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist) 
 : cfgBase("statusbar", "statusbar", parent, insert, paramlist) {

  setSavable(FALSE);
}


cfgStatusBar::~cfgStatusBar() {
}
