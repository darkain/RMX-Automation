/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cbList.h"
#include "cbListRoot.h"
#include "cbListItem.h"


cbList::cbList() {
}


cbList::~cbList() {
  cbListItem *cbitem = items.getFirstItem();
  while (cbitem) {
    delete cbitem;
    items.removeItem(cbitem);
    cbitem = items.getFirstItem();
  }

  cbListRoot *cbroot = roots.getFirstItem();
  while (cbroot) {
    delete cbroot;
    roots.removeItem(cbroot);
    cbroot = roots.getFirstItem();
  }
}


void cbList::cb_onInsertPlugin(HPLUGIN plugin) {
  new cbListRoot(this, plugin);
}
