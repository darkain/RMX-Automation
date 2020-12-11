/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/
#if 0


#include "../../../sdk/rmxBase.h"
#include "ConfigWnd.h"
#include "ConfigWndTree.h"


struct vTreePlugin {
  HPLUGIN plugin;
  HLIST   list;
};


//-----------------------------------------------------------------------------------


vConfigWndTree::vConfigWndTree(wndBase *parent) : wndTree(parent) {
}


wndTreeItem *vConfigWndTree::addItem(const char *name, wndTreeItem *parent, HPLUGIN plugin, HLIST list) {
  vTreePlugin data;
  data.plugin = plugin;
  data.list   = list;
  return addTreeItem(name, 0, parent, (UINT) &data);
}


wndTreeItem *vConfigWndTree::newTreeItem(const char *name, wndTreeItem *parent, UINT param) {
  if (param) {
    vTreePlugin *data = (vTreePlugin*)param;
    return new vPluginTreeItem(name, parent, data->plugin, (HLIST)data->list);
  }
  return NULL;
}


//-----------------------------------------------------------------------------------


vPluginTreeItem::vPluginTreeItem(const char *name, wndTreeItem *parent, HPLUGIN cb_plugin, HLIST cb_list)
 : wndTreeItem(name, parent), cbCore(cb_plugin, cb_list) {
}


vPluginTreeItem::~vPluginTreeItem() {
}


void vPluginTreeItem::onInit() {
  wndTreeItem::onInit();
  wndTreeItem::setExpanded( cbCore::isExpanded() );  //todo: find out why this doesnt work
}


//TODO: FIX THIS SHIT!!
void vPluginTreeItem::cb_onNameChange(const char *newname) {
  wndTreeItem::setName(newname);
//  cbCore::cb_onNameChange(newname);
}


void vPluginTreeItem::onSelected() {
  wndTreeItem::onSelected();
  vConfigWnd::getConfigWnd()->setActiveItem(this);
}


void vPluginTreeItem::onSetExpanded(BOOL expanded) {
  wndTreeItem::onSetExpanded(expanded);
  cbCore::setExpanded(expanded);

  if (expanded != cbCore::isExpanded()) {  //in case of a read-only list
    wndTreeItem::setExpanded( cbCore::isExpanded() );
  }
}


//TODO: FIX THIS SHIT!!
void vPluginTreeItem::cb_onSetExpanded(BOOL expanded) {
//  cbCore::cb_onSetExpanded(expanded);
  wndTreeItem::setExpanded(expanded);
}


void vPluginTreeItem::cb_onInsertChild(HLIST child, HLIST insert) {
  HPLUGIN plug = getRemotePlugin();
  if (plug) {
    const char *type = plug->list_getType(child);
    const char *name = plug->list_getName(child);
    if (type  &&  *type  &&  VSTRCMP(type, "tree") == 0) {
      wndTreeItem *item = new vPluginTreeItem(name, this, getRemotePlugin(), child);
      getwndBase()->addTreeItem(item);
    }
  }
}


void vPluginTreeItem::cb_onSetVisible(BOOL isvisible) {
  wndTreeItem::setVisible(isvisible);
}
#endif
