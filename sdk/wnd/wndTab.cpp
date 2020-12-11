/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "wndTab.h"


wndTabItem::wndTabItem(const char *name, wndTab *parent, wndTabItem *insert) {
  par = parent;
  par->items.insertItem(this, insert);

  rmxUtf8ToWide str(name);

  TCITEM tcitem;
  ZeroMemory(&tcitem, sizeof(TCITEM));
  tcitem.mask    = TCIF_TEXT | TCIF_PARAM;
  tcitem.pszText = (LPWSTR)str.getBuffer();
  tcitem.lParam  = (LPARAM)this;
  par->sendMessage(TCM_INSERTITEM, par->items.getItemIndex(this), (LPARAM)&tcitem);

  par->onInsertItem(this, insert);
}


wndTabItem::~wndTabItem() {
  par->onRemoveItem(this);
  par->sendMessage(TCM_DELETEITEM, par->items.getItemIndex(this));
  par->items.removeItem(this);
}


//-----------------------------------------------------------------------------------------------


wndTab::wndTab(wndBase *parent) : wndBase(WC_TABCONTROL, parent), whkNotify(parent) {
  setReturnDefault(FALSE);
  selected = NULL;
}


wndTab::~wndTab() {
  clearList();
}


void wndTab::updateSelectedTabItem() {
  TCITEM tcitem;
  ZeroMemory(&tcitem, sizeof(TCITEM));
  tcitem.mask = TCIF_PARAM;
  sendMessage(TCM_GETITEM, sendMessage(TCM_GETCURSEL), (UINT)&tcitem);

  wndTabItem *newselected = (wndTabItem*) tcitem.lParam;
  if (newselected != selected) {
    if (selected) selected->onDeselected();
    if (items.hasItem(newselected)) selected = newselected; else selected = NULL; 
    if (selected) selected->onSelected();
  }
}


void wndTab::clearList() {
  sendMessage(TCM_DELETEALLITEMS);

  wndTabItem *item = items.getFirstItem();
  while (item) {
    delete item;
    items.removeItem(item);
    item = items.getFirstItem();
  }
}


BOOL wndTab::onNotify(whkInfo *info, NMHDR *notify) {
  if (notify->hwndFrom == getWnd()) {
    if (notify->code == TCN_SELCHANGE) {
      updateSelectedTabItem();
    }
  }
  return whkNotify::onNotify(info, notify);
}


#endif
