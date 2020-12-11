/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/
#if 0


#include "../../../sdk/rmxBase.h"
#include "ConfigStatus.h"


wndConfigStatus::wndConfigStatus(wndBase *parent) : vButtonStatusWnd(parent) {
  HPLUGIN plugin = vHPLUGIN::getFirst();
  while (plugin) {
    new cbRootConfigStatus(plugin, this);
    plugin = vHPLUGIN::getNext(plugin);
  }
}


wndConfigStatus::~wndConfigStatus() {
  cbRootConfigStatus *item = roots.getFirstItem();
  while (item) {
    delete item;
    roots.removeItem(item);
    item = roots.getFirstItem();
  }
}


cbBaseConfigStatusButton *wndConfigStatus::getStatusButton(int id) {
  cbBaseConfigStatusButton *item = items.getFirstItem();
  while (item) {
    if (item->getId() == id) return item;
    item = items.getNextItem(item);
  }
  return NULL;
}


void wndConfigStatus::onAppendPlugin(vPlugin *plugin) {
  new cbRootConfigStatus((HPLUGIN)plugin, this);
}


VUINT wndConfigStatus::subWndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == WM_COMMAND) {
    int id = (LOWORD(param1));
    cbBaseConfigStatusButton *button = getStatusButton(id);
    if (button) button->message(GENERIC_CLICK);
    return 0;
  }
  return vButtonStatusWnd::subWndProc(msg, param1, param2);
}



//---------------------------------------------------------------------------------------------------------



cbRootConfigStatus::cbRootConfigStatus(HPLUGIN plugin, wndConfigStatus *par) : cbRoot(plugin), parent(par) {
  parent->roots.appendItem(this);

  HLIST child = plugin->root_getFirstChild();
  while (child) {
    const char *type = plugin->list_getType(child);
    if ( (type)  &&  (!VSTRCMP(type, "statusbar")) ) new cbBaseConfigStatusBar(plugin, child, this, parent);
    child = plugin->root_getNextChild(child);
  }
}


cbRootConfigStatus::~cbRootConfigStatus() {
  parent->roots.removeItem(this);

  cbBaseConfigStatusBar *item = items.getFirstItem();
  while (item) {
    delete item;
    items.removeItem(item);
    item = items.getFirstItem();
  }
}


void cbRootConfigStatus::cb_onInsertChild(HLIST child, HLIST insert) {
  const char *type = getRemotePlugin()->list_getType(child);
  if ( (type)  &&  (!VSTRCMP(type, "statusbar")) ) new cbBaseConfigStatusBar(getRemotePlugin(), child, this, parent);
}



//---------------------------------------------------------------------------------------------------------



cbBaseConfigStatusBar::cbBaseConfigStatusBar(HPLUGIN plugin, HLIST item, cbRootConfigStatus *par, wndConfigStatus *grandpar)
 : cbCore(plugin, item), parent(par), grandparent(grandpar) {

  parent->items.appendItem(this);

  HLIST child = plugin->list_getFirstChild(item);
  while (child) {
    const char *type = plugin->list_getType(child);
    if ( (type)  &&  (!VSTRCMP(type, "button")) ) new cbBaseConfigStatusButton(plugin, child, this, grandparent);
    child = plugin->list_getNextChild(item, child);
  }
}


cbBaseConfigStatusBar::~cbBaseConfigStatusBar() {
  parent->items.removeItem(this);

  cbBaseConfigStatusButton *item = items.getFirstItem();
  while (item) {
    delete item;
    items.removeItem(item);
    item = items.getFirstItem();
  }
}


void cbBaseConfigStatusBar::cb_onInsertChild(HLIST child, HLIST insert) {
  const char *type = getRemotePlugin()->list_getType(child);
  if ( (type)  &&  (!VSTRCMP(type, "button")) ) new cbBaseConfigStatusButton(getRemotePlugin(), child, this, grandparent);
}



//---------------------------------------------------------------------------------------------------------


int cbBaseConfigStatusButton::btns = 1;


cbBaseConfigStatusButton::cbBaseConfigStatusButton(HPLUGIN plugin, HLIST item, cbBaseConfigStatusBar *par, wndConfigStatus *grandpar)
 : cbCore(plugin, item), parent(par), grandparent(grandpar) {

  parent->items.appendItem(this);
  grandparent->items.appendItem(this);

  id = btns++;

  parent->getParent()->getParent()->addButton(plugin->list_getName(item), id);
}


cbBaseConfigStatusButton::~cbBaseConfigStatusButton() {
  parent->getParent()->getParent()->removeButton(id);
  parent->items.removeItem(this);
  grandparent->items.removeItem(this);
}


void cbBaseConfigStatusButton::cb_cfgMessage(cbMessage message, HPARAM param, HPARAM insert) {
	cbCore::cb_cfgMessage(message, param, insert);

	if (isParamUserData(param)) {
		wndBase *wnd = parent->getParent()->getParent()->getButton(id);
		if (wnd) wnd->setName( getRemotePlugin()->param_getValue(param) );
	}
}

/*
void cbBaseConfigStatusButton::cb_onNameChange(const char *newname) {
  cbBase::cb_onNameChange(newname);

  wndBase *wnd = parent->getParent()->getParent()->getButton(id);
  if (wnd) wnd->setName(newname);
}
*/
#endif
