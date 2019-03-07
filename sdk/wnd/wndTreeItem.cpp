/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "wndTreeItem.h"


wndTreeItem::wndTreeItem(const char *name, wndTreeItem *parent, BOOL isvisible) : rmxString(name) {
  treeParent = parent;
  treeRoot   = NULL;
  hTree      = NULL;
  hItem      = NULL;

  visible    = isvisible;
  expand     = FALSE;

  if (treeParent) {
    treeRoot = treeParent->getTreeRoot();
    treeParent->addTreeChild(this);
  } else {
    treeRoot = this;
  }
}


wndTreeItem::~wndTreeItem() {
  wndTreeItem *itm = items.getFirstItem();
  while (itm) {
    items.removeItem(itm);
    delete itm;
    itm = items.getFirstItem();
  }

  if (treeParent)  treeParent->delTreeChild(this);
  if (wnd) {
    if (wnd->selected == this) wnd->selected = NULL;
    wnd->delTreeChild(this);
  }

  if (hItem) TreeView_DeleteItem(hTree, hItem);
}


void wndTreeItem::select() {
  TreeView_SelectItem(hTree, hItem);
}


void wndTreeItem::setExpanded(BOOL isexpand) {
  if (isexpand == expand) return;
  expand = isexpand;

  TreeView_Expand( hTree, hItem, (expand ? TVE_EXPAND : TVE_COLLAPSE) );
  onSetExpanded(expand);
}


BOOL wndTreeItem::isExpanded() {
/*  TVITEM tvitem;
  ZeroMemory(&tvitem, sizeof(TVITEM));
  tvitem.mask      = TVIF_STATE;
  tvitem.stateMask = TVIS_EXPANDED;
  tvitem.hItem     = hItem;
  TreeView_GetItem(hTree, &tvitem);
  return (tvitem.state & TVIS_EXPANDED) > 0;*/
  return expand;
}


void wndTreeItem::setVisible(BOOL isvisible) {
  if (isvisible == visible) return;
  visible = isvisible;

  if (visible) {
    rmxUtf8ToWide str(*this);
    
    TVINSERTSTRUCT a;
    ZeroMemory(&a, sizeof(a));

    if (getTreeParent()) {
      a.hParent     = getTreeParent()->getTreeItem();
    } else {
      a.hParent     = NULL;
    }
    a.hInsertAfter  = TVI_LAST;  //TODO:  we can actually pull the prev/next items for this
    a.item.mask     = TVIF_TEXT | TVIF_PARAM | TVIF_STATE;
    a.item.pszText  = (LPWSTR)str.getBuffer();
    a.item.lParam   = (LPARAM)this;
    a.item.state    = (expand ? TVIS_EXPANDED : 0);

    HTREEITEM hItem = (HTREEITEM)wnd->sendMessage(TVM_INSERTITEM, NULL, (LPARAM)&a);
    setTreeItem(hItem);
  }

  else {
    TreeView_DeleteItem(hTree, hItem);
    setTreeItem(NULL);
  }
}


BOOL wndTreeItem::isVisible() {
  return visible;
}


void wndTreeItem::setWnd(wndTree *w) {
  wnd = w;
  if (wnd) {
    hTree = w->getWnd();
  } else {
    hTree = NULL;
  }
}


wndTree *wndTreeItem::getwndBase() {
  return wnd;
}


void wndTreeItem::addTreeChild(wndTreeItem *item) {
  items.appendItem(item);
}

void wndTreeItem::delTreeChild(wndTreeItem *item) {
  items.removeItem(item);
}

void wndTreeItem::onDeselected() {
  wnd->selected = NULL;
}

void wndTreeItem::onSelected() {
  wnd->selected = this;
}

void wndTreeItem::onNameChange(const char *newname) {
  if (!hTree) return;
  if (!hItem) return;

  rmxUtf8ToWide str(*this);

  TVITEMW tvitem;
  ZeroMemory(&tvitem, sizeof(TVITEM));
  tvitem.mask    = TVIF_TEXT;
  tvitem.hItem   = hItem;
  tvitem.pszText = (LPWSTR)str.getBuffer();
  wnd->sendMessage(TVM_SETITEM, 0, (LPARAM)&tvitem);
}


#endif
