/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/cfgHelper.h"
#include "menuItems.h"


static vMenuPlugin *MENU = new vMenuPlugin();


vMenuPlugin::vMenuPlugin() : plugBase("Menu Items") {
  tree = NULL;
}


void vMenuPlugin::onInit() {
  tree = new MenuTree(getName(), this);
  new tcfgAddChildExpand<MenuItem>("Add Menu Item", "Menu Item", tree);
  new cfgLine(tree);
}


void vMenuPlugin::onQuit() {
  vdelnull(tree);
}


//---------------------------------------------------------------------------------------------------------------------


//void MenuTree::onMoveChild(const cfgBase *child, const cfgBase *insert) {
//  MenuTreeParent::onMoveChild(child, insert);
void MenuTree::cb_cfgMessage(cbMessage message, cfgBase *item, const cfgBase *child, const cfgBase *insert) {
	if (message == CB_CHILD_MOVE) {
		cfgBase *item1 = (isUnique(child )) ? ((RMX_CAST<const MenuItem*>(child ))->getMenu()) : (NULL);
		cfgBase *item2 = (isUnique(insert)) ? ((RMX_CAST<const MenuItem*>(insert))->getMenu()) : (NULL);

		if (item1) getListRoot()->moveChild(item1, item2);
	}

	MenuTreeParent::cb_cfgMessage(message, item, child, insert);
}


//---------------------------------------------------------------------------------------------------------------------


MenuItem::MenuItem(const char *name, cfgBase *parent, HPARAMLIST paramlist)
 : evntSingle(name, parent, NULL, paramlist) {

  text = NULL;
  menuitem = new MenuItemClick(this);
  text = new MenuText(this);
  text->setText("Menu Item");

  new tcfgMoveUniqueUp<  MenuTreeParent>("Move Up",   "Move Up",   this);
  new tcfgMoveUniqueDown<MenuTreeParent>("Move Down", "Move Down", this);
  new cfgRemoveParent("Remove Menu Item", this);
}


MenuItem::~MenuItem() {
  vdelnull(text);
  vdelnull(menuitem);
}


void MenuItem::updateText() {
  if (text) menuitem->setName(text->getText());
}
