/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "wndDroplist.h"


wndDroplist::wndDroplist(wndBase *parent) : wndSubclass("ComboBox", parent) {
  whkCommand::insertHookWnd(this);
}


wndDroplist::~wndDroplist() {
  vDroplistItem *itm = listItems.getFirstItem();
  while (itm) {
    delete itm;
    listItems.removeItem(itm);
    itm = listItems.getFirstItem();
  }
}


vDroplistItem *wndDroplist::newDroplistItem(const char *name, void *data) {
  return new vDroplistItem(name, data);
}


vDroplistItem *wndDroplist::addDroplistItem(const char *name, void *data) {
  vDroplistItem *item = newDroplistItem(name, data);
  if (item == NULL) return NULL;
  item->setWnd(this);

  rmxUtf8ToWide string(name);

  VINT pos = (UINT)sendMessage(CB_ADDSTRING, 0, (VUINT)string.getBuffer());
  sendMessage(CB_SETITEMDATA, pos, (VUINT)item);
  if (item->getInitialSelect()) sendMessage(CB_SETCURSEL, pos);

  listItems.appendItem(item);
  return item;
}


vDroplistItem *wndDroplist::getSelectedItem() {
  VUINT sel = (VUINT)sendMessage(CB_GETCURSEL);
  if (sel == CB_ERR) return NULL;
  return (vDroplistItem*) sendMessage(CB_GETITEMDATA, sel);
}


void wndDroplist::delDroplistItem(vDroplistItem *item) {
  listItems.removeItem(item);
}


void wndDroplist::clearDroplist() {
  sendMessage(CB_RESETCONTENT);
}


void wndDroplist::onSelectionChange() {
  VINT pos = (VUINT) sendMessage(CB_GETCURSEL);
  vDroplistItem *item = (vDroplistItem*) sendMessage(CB_GETITEMDATA, pos);
  if (listItems.hasItem(item)) item->onSelected();
}


BOOL wndDroplist::onCommand(whkInfo *info) {
  if (info  &&  HIWORD(info->param1) == LBN_SELCHANGE) {
    onSelectionChange();
  }
  return whkCommand::onCommand(info);
}


//------------------------------------------------------------------------------


vDroplistItem::vDroplistItem(const char *name, void *thedata) : rmxString(name) {
  data = thedata;
  initialSelect = FALSE;
  wnd = NULL;
}


vDroplistItem::~vDroplistItem() {
  if (wnd) wnd->delDroplistItem(this);
}


void vDroplistItem::selectItem() {
  if (!wnd) return;
  VINT pos = getPosition();
  if (pos == -1) return;
  wnd->sendMessage(CB_SETCURSEL, pos);
}


int vDroplistItem::getPosition() {
  if (!wnd) return -1;

  rmxUtf8ToWide string(*this);

  VINT result = (VINT) wnd->sendMessage(CB_FINDSTRINGEXACT, -1, (VUINT)string.getBuffer());
  if (result == CB_ERR) return -1;

  return result;
}


#endif
