/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "GameSlider.h"


vGameSlider::vGameSlider(GUID guid, const char *name, cfgBase *parent)
 : evntAbsolute(name, parent, NULL) {

  insertParam("unique", "slider");
  insertParam("guid", NULLSTR)->setValueGuid(guid);

  setMinMax(0, 10000);
}


vGameSlider::~vGameSlider() {
}
