/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "wndTree.h"
#include "wndTreeItem.h"


wndTree::wndTree(wndBase *parent) : wndBase("SysTreeView32", parent) {
  appendWindowStyleEx(WS_EX_CLIENTEDGE);
  selected = NULL;
  insertHookWnd(parent);
}


wndTree::~wndTree() {
  clearList();
}


void wndTree::clearList() {
  wndTreeItem *itm = items.getFirstItem();
  while (itm) {
    items.removeItem(itm);
    delete itm;
    itm = items.getFirstItem();
  }
  TreeView_DeleteAllItems(getWnd());
}


wndTreeItem *wndTree::newTreeItem(const char *name, wndTreeItem *parent, UINT param) {
  return new wndTreeItem(name, parent);
}


void wndTree::addTreeChild(wndTreeItem *item) {
  items.appendItem(item);
}

void wndTree::delTreeChild(wndTreeItem *item) {
  items.removeItem(item);
}


wndTreeItem *wndTree::addTreeItem(const char *name, UINT notifyId, wndTreeItem *parent, UINT param) {
  wndTreeItem *treeItem = newTreeItem(name, parent, param);
  if (treeItem == NULL) return NULL;

  addTreeItem(treeItem);
  return treeItem;
}


void wndTree::addTreeItem(wndTreeItem *treeItem) {
  treeItem->setWnd(this);

  if (treeItem->isVisible()) {  //TODO:  move this body into its own function
    rmxUtf8ToWide str(*treeItem);
    
    TVINSERTSTRUCT a;
    if (treeItem->getTreeParent()) a.hParent = treeItem->getTreeParent()->getTreeItem();
    else a.hParent  = NULL;
    a.hInsertAfter  = TVI_LAST;  //TODO:  we can actually pull the prev/next items for this
    a.item.mask     = TVIF_TEXT | TVIF_PARAM;
    a.item.pszText  = (LPWSTR)str.getBuffer();
    a.item.lParam   = (LPARAM)treeItem;

    HTREEITEM hItem = (HTREEITEM)sendMessage(TVM_INSERTITEM, NULL, (LPARAM)&a);
    treeItem->setTreeItem(hItem);
  }

  items.appendItem(treeItem);
}


BOOL wndTree::onNotify(whkInfo *info, NMHDR *notify) {
  NMTREEVIEW *tree_notify = (NMTREEVIEW*) notify;

  if (info->param1 == getId()) {
    if (notify->code == TVN_SELCHANGED) {
      wndTreeItem *prewndTreeItem = (wndTreeItem*)tree_notify->itemOld.lParam;
      if (prewndTreeItem  &&  items.hasItem(prewndTreeItem)) {
        prewndTreeItem->onDeselected();
      }

      wndTreeItem *nextTreeItem = (wndTreeItem*)tree_notify->itemNew.lParam;
      if (nextTreeItem  &&  items.hasItem(nextTreeItem)) {
        nextTreeItem->onSelected();
      }

      setFocus();
    }

    else if (notify->code == TVN_ITEMEXPANDED) {
      wndTreeItem *item = (wndTreeItem*)tree_notify->itemNew.lParam;
      if (item  &&  items.hasItem(item)) {
        item->onSetExpanded(item->isExpanded());
      }
    }
  }
  return FALSE;
}


#endif

