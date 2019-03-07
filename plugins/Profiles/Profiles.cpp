/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "Profiles.h"


static vProfiles *PROFILES = new vProfiles();


vProfiles::vProfiles() : plugBase("Profiles") {
  function = NULL;
  group    = NULL;
  tree     = NULL;
  active   = NULL;
}


void vProfiles::onInit() {
  active = new cfgText("active",  this);
  tree   = new vProfileTreeRoot(getName(), this);

  new vAddTree(tree);
  new vAddItem(tree);
  new cfgLine(tree);

  function = new funcSingleProfile(this);
  group    = new funcGroup(getName(), "profiles", this);
}


void vProfiles::onQuit() {
}


void vProfiles::onPreLoadSettings() {
  vProfileTree *tree = trees.getFirstItem();
  while (tree) {
    delete tree;
    trees.removeItem(tree);
    tree = trees.getNextItem(tree);
  }

  vProfileItem *item = items.getFirstItem();
  while (item) {
    delete item;
    items.removeItem(item);
    item = items.getNextItem(item);
  }

  plugBase::onPreLoadSettings();
}


vProfileTree *vProfiles::getActiveTree() {
  if (!active) return NULL;
  const char *name = active->getUserData();
  if (!name) return NULL;

  vProfileTree *tree = trees.getFirstItem();
  while (tree) {
    if (VSTRCMP(name, tree->getName()) == 0) return tree;
    tree = trees.getNextItem(tree);
  }

  return NULL;
}


funcParam *vProfiles::getFunctionParam() {
  return function->getFunctionParam();
}



//-----------------------------------------------------------


vProfileTreeRoot::vProfileTreeRoot(const char *name, cfgBase *parent) : cfgTree(name, parent) {
}


vProfileTreeRoot::~vProfileTreeRoot() {
}


cfgBase *vProfileTreeRoot::onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist) {
  if (type) {
    if (VSTRCMP(type, "tree") == 0) {
      return new vProfileTree(name, this);
    }

    if (VSTRCMP(type, "string") == 0) {
      return new vProfileItem(this);
    }
  }
  return cfgTree::onNewExternalChild(name, type, paramlist);
}


//-----------------------------------------------------------


funcSingleProfile::funcSingleProfile(cfgRoot *parent) : funcSingle("Set Active Profile", "setProfile", parent) {
  param = new funcParam("Profile", "f_profile", "stringlist", this);
}

funcSingleProfile::~funcSingleProfile() {
}

void funcSingleProfile::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  const char *profile = source->list_getParam(input, "f_profile");
  if (profile) PROFILES->setActiveTree(profile);
}


//-----------------------------------------------------------


vProfileTree::vProfileTree(const char *name, cfgBase *parent) : cfgTree(name, parent) {
  param = PROFILES->getFunctionParam()->addListItem(getName());

  new cfgValidateRenameParent("Profile name", this);
  new cfgRemoveParent("Remove profile item", this);
  new cfgLine(this);

  vProfileItem *item = PROFILES->items.getFirstItem();
  while (item) {
    addItem(item);
    item = PROFILES->items.getNextItem(item);
  }

  PROFILES->trees.appendItem(this);
}


vProfileTree::~vProfileTree() {
  PROFILES->trees.removeItem(this);
}


void vProfileTree::onNameChange(const char *newname) {
  cfgTree::onNameChange(newname);
  param->setName(newname);
}


void vProfileTree::addItem(vProfileItem *item) {
  new vProfileTreeItem(item->getUserData(), this, item);
}


evntSingle *vProfileTree::getEvent(cfgBase *item) {
  vProfileTreeItem *treeitem = items.getFirstItem();
  while (treeitem) {
    if (item == treeitem->getConfig()) return treeitem;
    treeitem = items.getNextItem(treeitem);
  }

  return NULL;
}


//-----------------------------------------------------------


vProfileTreeItem::vProfileTreeItem(const char *name, vProfileTree *parent, cfgBase *watch)
 : evntSingle(name, parent), cbBase(watch) {
  par = parent;
  par->appendTreeItem(this);
}


vProfileTreeItem::~vProfileTreeItem() {
  par->removeTreeItem(this);
}


void vProfileTreeItem::cb_onSetUserData(const prmBase *userdata) {
  cbBase::cb_onSetUserData(userdata);
  setName( userdata->getValue() );
}


//-----------------------------------------------------------


void vAddTree::onClick(HPLUGIN source) {
  cfgButton::onClick(source);

  char itemtext[32];
  int i=1;
  while (i < 9999) {
    VPRINTF(itemtext, sizeof(itemtext), "Profile %d", i);
    if (!getParent()->getChild(itemtext)) break;
    i++;
  }

  getParent()->setExpanded(TRUE);
  new vProfileTree(itemtext, getParent());
}


//-----------------------------------------------------------


vProfileItem::vProfileItem(cfgBase *parent) : cfgText("Profile Item", parent, PROFILES->trees.getFirstItem()) {
  char itemtext[32];
  int i=1;
  while (i < 9999) {
    VPRINTF(itemtext, sizeof(itemtext), "Profile Item %d", i);
    if (!parent->getChildByUserData(itemtext)) break;
    i++;
  }

  oldval.setName(itemtext);

  function = new vItemFunction(itemtext, PROFILES->getGroup(), this);
  new cfgRemoveParent("Remove profile item", this);
  
  setText(itemtext);

  PROFILES->items.appendItem(this);

  vProfileTree *tree = PROFILES->trees.getFirstItem();
  while (tree) {
    tree->addItem(this);
    tree = PROFILES->trees.getNextItem(tree);
  }
}


vProfileItem::~vProfileItem() {
  PROFILES->items.removeItem(this);
  if (PROFILES->getGroup()->hasChild(function)) vdelnull(function);
}


void vProfileItem::validateUserData(prmBase *userdata) {
  cfgBase *thislist = this;
  cfgBase *list = getParent()->getFirstChild();
  while (list) {
    if (list != thislist) {
      const char *data = list->getUserData();
      if (data  &&  VSTRCMP(userdata->getValue(), data)==0) {
        userdata->setValue(oldval.getName());
        return;
      }
    }
    list = getParent()->getNextChild(list);
  }
  oldval.setName(userdata->getValue());
}


void vProfileItem::onSetUserData(const prmBase *userdata) {
  cfgText::onSetUserData(userdata);
  if (PROFILES->getGroup()->hasChild(function)) function->setName( userdata->getValue() );
}


//-----------------------------------------------------------


void vAddItem::onClick(HPLUGIN source) {
  cfgButton::onClick(source);
  new vProfileItem(getParent());
  vHPLUGIN::getMaster()->root_message(FUNCTION_START);
}


//-----------------------------------------------------------



vItemFunction::vItemFunction(const char *name, funcGroup *parent, cfgBase *item) : funcSingle (name, NULL, parent) {
  profileitem = item;
}


vItemFunction::~vItemFunction() {
}


void vItemFunction::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  vProfileTree *tree = PROFILES->getActiveTree();
  if (!tree) return;

  evntSingle *event = tree->getEvent(profileitem);
  if (event) event->activate();
}
