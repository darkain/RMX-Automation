/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "Playlist.h"
#include "Resource.h"

#include <commctrl.h>


//static vPlaylistPlugin plugin;
static vPlaylistPlugin *plugin = new vPlaylistPlugin();



vPlaylistPlugin::vPlaylistPlugin() : plugBase("Playlist") {
  root = NULL;
}

void vPlaylistPlugin::onInit() {
  root = new cfgTree(getName());

  wnd = new vPlaylistWnd(getName(), root);
  wnd->init();
}

void vPlaylistPlugin::onQuit() {
  delete wnd;
  wnd = NULL;

  delete root;
  root = NULL;
}


void vPlaylistPlugin::onPostInit() {
  wnd->refreshList();
//  wnd->showWindow();
}


//-------------------------------------------------------------------------------


vPlaylistWnd::vPlaylistWnd(const char *name, cfgBase *parent)
: wndBase("vPlaylistWnd"), cfgBool("Visible", parent) {

  tabwnd  = NULL;
  listwnd = NULL;

  wndBase::setName(name);
  resize(300, 200, FALSE);
}


vPlaylistWnd::~vPlaylistWnd() {
}


void vPlaylistWnd::onInit() {
  wndBase::onInit();

  tabwnd  = new wndTab( this);
  listwnd = new wndList(this);

  initChildren();
  showChildren();
}


void vPlaylistWnd::onResize(int width, int height) {
  int size = 21;
  if (tabwnd) tabwnd->moveresize(0, 0, width, size);
  if (listwnd) listwnd->moveresize(0, size, width, height-size);
}


//void vPlaylistWnd::onSetUserData(const prmBase *newdata) {
//  cfgBool::onSetUserData(newdata);
//  wndBase::setVisible(getValue());
//}

void vPlaylistWnd::cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert) {
	wndBase::setVisible( getValue() );
	cfgBool::cb_cfgMessage(message, item, param, insert);
}



void vPlaylistWnd::refreshList() {
  tabwnd->clearList();

  HPLUGIN plugin = vHPLUGIN::getFirst();
  while (plugin) {
    HLIST list = plugin->root_getFirstChild();
    while (list) {
      if (VSTRCMP(plugin->list_getType(list), "playlist") == 0) {
        const char *name = plugin->list_getName(list);
        vPlaylistTabItem *item = new vPlaylistTabItem(name, plugin, list, tabwnd);
      }
      list = plugin->root_getNextChild(list);
    }
    plugin = vHPLUGIN::getNext(plugin);
  }

  wndTabItem *item = tabwnd->getSelected();
  if (item) static_cast<vPlaylistTabItem*>(item)->refreshList(listwnd);
}


//-------------------------------------------------------------------------------


vPlaylistTabItem::vPlaylistTabItem(const char *name, HPLUGIN hplugin, HLIST hlist, wndTab *parent) : wndTabItem(name, parent) {
  plugin = hplugin;
  list = hlist;
}


vPlaylistTabItem::~vPlaylistTabItem() {
}


void vPlaylistTabItem::onSelected() {
  vPlaylistWnd *wnd = static_cast<vPlaylistWnd*>(getParent()->getParentWnd());
  refreshList(wnd->getListWnd());
}


void vPlaylistTabItem::refreshList(wndList *listwnd) {
  listwnd->clearList();

  HLIST child = plugin->list_getFirstChild(list);
  while (child) {
    if (VSTRCMP(plugin->list_getType(child), "plentry") == 0) {
	  HPARAM param = plugin->list_getListParam(child);
	  new vPlaylistListItem(plugin->param_getValueSafe(param), plugin, child, listwnd);
    }
    child = plugin->list_getNextChild(list, child);
  }
}


//-------------------------------------------------------------------------------


vPlaylistListItem::vPlaylistListItem(const char *name, HPLUGIN hplugin, HLIST hlist, wndList *parent) : wndListItem(name, parent) {
  plugin = hplugin;
  list = hlist;
}

vPlaylistListItem::~vPlaylistListItem() {
}

void vPlaylistListItem::onDoubleClick() {
  HLIST hpar = plugin->list_getParent(list);
  plugin->list_setParamInt(hpar, "pos", (int)list);  //todo: see why this is INT
  plugin->list_setParam(hpar, "status", "play");
}

