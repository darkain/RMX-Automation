/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/
#if 0

#include "../../../sdk/rmxBase.h"
#include "../../../sdk/wnd/wndForestItem.h"
#include "ConfigPaneItem.h"
#include "ConfigPaneWnd.h"


vConfigPaneItem::vConfigPaneItem(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list)
 : cbCoreDelay(cb_plugin, cb_list) {

  pane = panewnd;
  item = NULL;
  if (pane) pane->addPaneItem(this);
}


vConfigPaneItem::~vConfigPaneItem() {
  if (pane) pane->delPaneItem(this);
}


void vConfigPaneItem::setForestItem(wndForestItem *fitem) {
  item = fitem;
  if (!item) return;

  item->setName(getName());
  item->setExpanded(isExpanded());
  item->setHighlighted(isHighlighted());
  item->setItemEnabled(isEnabled());
  item->setVisible(isVisible());
}


void vConfigPaneItem::onInit() {
  if (item) {
//    const char *tip = getTooltip();
//    if (tip  &&  *tip) item->setTooltip(tip);
  }
}


void vConfigPaneItem::cb_cfgMessage(cbMessage message, const char *text) {
	cbCoreDelay::cb_cfgMessage(message, text);
}


void vConfigPaneItem::cb_cfgMessage(cbMessage message, RMXLONG data) {
	cbCoreDelay::cb_cfgMessage(message, data);
}


void vConfigPaneItem::cb_cfgMessage(cbMessage message, HLIST child, HLIST insert) {
	cbCoreDelay::cb_cfgMessage(message, child, insert);
}

void vConfigPaneItem::cb_cfgMessage(cbMessage message, HPARAM child, HPARAM insert) {
	cbCoreDelay::cb_cfgMessage(message, child, insert);
}


/*
void vConfigPaneItem::cb_onNameChange(const char *newname) {
  if (item) item->setName(newname);
  cbCoreDelay::cb_onNameChange(newname);
}


void vConfigPaneItem::cb_onInsertChild(HLIST child, HLIST insert) {
  wndForestBase *obj = NULL;
  if (pane) pane->getObjectFromList((HLIST)insert);
  if (item) pane->enumChildren((HLIST)child, item, obj);
  cbCoreDelay::cb_onInsertChild(child, insert);
}


void vConfigPaneItem::cb_onSetVisible(BOOL visible) {
  if (item) item->setVisible(visible);
  cbCoreDelay::cb_onSetVisible(visible);
}


void vConfigPaneItem::cb_onSetEnabled(BOOL enabled) {
  if (item) item->setItemEnabled(enabled);
  cbCoreDelay::cb_onSetEnabled(enabled);
}


void vConfigPaneItem::cb_onSetExpanded(BOOL expanded) {
  if (item) item->setExpanded(expanded);
  cbCoreDelay::cb_onSetExpanded(expanded);
}


void vConfigPaneItem::cb_onSetHighlighted(BOOL highlighted) {
  if (item) item->setHighlighted(highlighted);
  cbCoreDelay::cb_onSetHighlighted(highlighted);
}


void vConfigPaneItem::cb_onMoveChild(HLIST child, HLIST insert) {
  if (!item  ||  !pane) return;
  item->moveForestChild(pane->getObjectFromList(child), pane->getObjectFromList(insert));
  cbCoreDelay::cb_onMoveChild(child, insert);
}
*/

#endif
