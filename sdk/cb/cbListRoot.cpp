/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cbListRoot.h"
#include "cbList.h"


cbListRoot::cbListRoot(cbList *parent, HPLUGIN plugin) : cbRoot(plugin) {
  list = parent;
  list->roots.appendItem(this);

  HLIST root = plugin->list_getRoot();
  HLIST child = plugin->list_getFirstChild(root);
  while (child) {
    cbListItem *cb = list->validateConfig(child);
    if (cb) list->items.appendItem(cb);
    child = plugin->list_getNextChild(root, child);
  }
}


cbListRoot::~cbListRoot() {
  list->roots.removeItem(this);
}


void cbListRoot::cb_onInsertChild(HLIST child, HLIST insert) {
  cbListItem *cb = list->validateConfig(child);
  if (cb) list->items.appendItem(cb);
}
