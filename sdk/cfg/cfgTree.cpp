/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cfgTree.h"


cfgTree::cfgTree(const char *itemName, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : cfgBase(itemName, "tree", parent, insert, paramlist) {
	setParam("title", "")->setSavable(false);
}


cfgTree::~cfgTree() {
}
