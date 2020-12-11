/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "wndList.h"


wndListItem::wndListItem(const char *name, wndList *parent, wndListItem *insert) {
  par = parent;
  par->items.insertItem(this, insert);

  VINT index = par->sendMessage(LB_INSERTSTRING, par->items.getItemIndex(this), (UINT)name);
  par->sendMessage(LB_SETITEMDATA, index, (UINT)this);
}


wndListItem::~wndListItem() {
  par->sendMessage(LB_DELETESTRING, par->items.getItemIndex(this));
  par->items.removeItem(this);
}


//-----------------------------------------------------------------------------------------------


wndList::wndList(wndBase *parent) : wndBase("LISTBOX", parent), whkCommand(parent) {
  appendWindowStyle(WS_VSCROLL | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY);
  setReturnDefault(FALSE);
  selected = NULL;
}


wndList::~wndList() {
  clearList();
}


void wndList::clearList() {
  sendMessage(LB_RESETCONTENT);
  wndListItem *item = items.getFirstItem();
  while (item) {
    delete item;
    items.removeItem(item);
    item = items.getFirstItem();
  }
}


BOOL wndList::onCommand(whkInfo *info) {
  if (info->param2 == (UINT)getWnd()) {
    if (HIWORD(info->param1) == LBN_SELCHANGE) {
      if (selected) selected->onDeselected();
      selected = (wndListItem*) sendMessage(LB_GETITEMDATA, sendMessage(LB_GETCURSEL));
      if (!items.hasItem(selected)) selected = NULL;
      if (selected) selected->onSelected();
    }
    
    else if (HIWORD(info->param1) == LBN_DBLCLK) {
      wndListItem *item = (wndListItem*) sendMessage(LB_GETITEMDATA, sendMessage(LB_GETCURSEL));
      if (items.hasItem(item)) item->onDoubleClick();
    }
  }
  return whkCommand::onCommand(info);
}


#endif
