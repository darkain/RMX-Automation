/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cfgLink.h"


cfgLink::cfgLink(const char *itemName, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : cfgNull(itemName, "link", parent, insert, paramlist) {

  setSavable(FALSE);
}


cfgLink::~cfgLink() {
}
