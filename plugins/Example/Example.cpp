/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "Example.h"


static plugExample *example = new plugExample();


plugExample::plugExample() : plugBase("Example Plugin") {
}


void plugExample::onInit() {
  cfgTree *tree = new cfgTree(getName());
  new cfgText("Hello World", tree);
}
