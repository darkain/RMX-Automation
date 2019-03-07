/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cbListItem.h"
#include "cbList.h"


cbListItem::cbListItem(cbList *parent, HPLUGIN plugin, HLIST item) : cbCore(plugin, item) {
  parlist = parent;
}


cbListItem::~cbListItem() {
  parlist->items.removeItem(this);
}
