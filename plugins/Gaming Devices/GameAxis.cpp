/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "GameAxis.h"


vGameAxis::vGameAxis(GUID guid, const char *name, cfgBase *parent)
 : evntRelative(name, parent, NULL) {

  insertParam("unique", "axis");
  insertParam("guid", NULLSTR)->setValueGuid(guid);

  setMinMax(-10000, 10000);
}


vGameAxis::~vGameAxis() {
}
