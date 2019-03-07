/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "Tray.h"


static vTrayPlugin *tray = new vTrayPlugin();


vTrayPlugin::vTrayPlugin() : plugBase("Tray Icons") {
  tree = NULL;
}


void vTrayPlugin::onInit() {
  tree = new vTrayRoot(getName(), this);
  new tcfgAddChildExpand<vTrayPluginIcon>("Add Tray Icon", "Tray Icon", tree);
  new cfgLine(tree);
}


void vTrayPlugin::onQuit() {
  vdelnull(tree);
}


void vTrayPlugin::updateIcons() {
  cfgBase *child = tree->getFirstUnique();
  while (child) {
    vTrayPluginIcon *icon = RMX_CAST<vTrayPluginIcon*>(child);
    if (icon) icon->update();
    child = tree->getNextUnique(child);
  }
}


//---------------------------------------------------------------------------------------------------------------------


void vTrayRoot::cb_cfgMessage(cbMessage message, cfgBase *item, const cfgBase *child, const cfgBase *insert) {
  tray->updateIcons();
  TrayTreeParent::cb_cfgMessage(message, item, child, insert);
}
/*
void vTrayRoot::onMoveChild(const cfgBase *child, const cfgBase *insert) {
  TrayTreeParent::onMoveChild(child, insert);
}
*/

//---------------------------------------------------------------------------------------------------------------------


vTrayPluginIcon::vTrayPluginIcon(const char *name, cfgBase *parent, HPARAMLIST paramlist)
 : evntSingle(name, parent, NULL, paramlist) {

  vTrayIcon::init();
  vTrayIcon::setVisible(TRUE);

  path = new vTrayPath(this);


  new tcfgMoveUniqueUp<  TrayTreeParent>("Move Up",   "Move Up",   this);
  new tcfgMoveUniqueDown<TrayTreeParent>("Move Down", "Move Down", this);
  new cfgRemoveParent("Remove Icon", this);
}


vTrayPluginIcon::~vTrayPluginIcon() {
}


void vTrayPluginIcon::onRightClick(int x, int y) {
  HPLUGIN plugin = vHPLUGIN::getMaster();
  plugin->list_message(plugin->list_getRoot(), MENU_MASTER);
}


void vTrayPluginIcon::update() {
  vTrayIcon::setVisible(FALSE);
  if (cfgBase::isVisible()) vTrayIcon::setVisible(TRUE);
}
