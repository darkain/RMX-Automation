/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

//todo:  expand menu before adding item, instead of after, as to not de-sync windows

#include "Macro.h"


static vMacro *MACRO = new vMacro();


vMacro::vMacro() : plugBase("Macro") {
}


void vMacro::onInit() {
  cfgTree *tree = new vMacroTree(getName(), this);

  group = new funcGroup("Macro", NULL, this);
}


//--------------------------------------------------------------


vMacroTree::vMacroTree(const char *name, cfgBase *parent) : cfgTree(name, parent) {
  new vAddMacro(this);
}


vMacroTree::~vMacroTree() {
}


cfgBase *vMacroTree::onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist) {
  if (type  &&  VSTRCMP(type, "tree")==0) {
    return new vMacroList(name, this);
  }
  return cfgTree::onNewExternalChild(name, type, paramlist);
}


void vMacroTree::onPreLoadSettings() {
  cfgBase *child = getFirstChild();
  while (child) {
    cfgBase *next = getNextChild(child);
    if (VSTRCMP(child->getType(), "tree")==0) {
      delete child;
    }
    child = next;
  }

  cfgTree::onPreLoadSettings();
}


//--------------------------------------------------------------


vMacroList::vMacroList(const char *name, cfgBase *parent) : cfgTree(name, parent) {
  function = new vMacroFunction(name, MACRO->getGroup(), this);
  new cfgValidateRenameParent("Macro name", this);
  new cfgRemoveParent("Remove Macro", this);
  new vAddMacroItem(this);
  new cfgLine(this);

  initConfig(); //TODO: temporary fix
}


vMacroList::~vMacroList() {
  vdelnull(function);
}


cfgBase *vMacroList::onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist) {
  if (type  &&  VSTRCMP(type, "button")==0) {
    return addEvent();
  }
  return cfgTree::onNewExternalChild(name, type, paramlist);
}


void vMacroList::onRemoveChild(const cfgBase* child) {
  evntSingle *event = static_cast<evntSingle*>((cfgBase*)child);
  events.removeItem(event);
  cfgTree::onRemoveChild(child);
}


void vMacroList::onNameChange(const char *newname) {
  cfgTree::onNameChange(newname);
  function->setName(newname);
}


cfgBase *vMacroList::addEvent(const char *name) {
  char itemtext[32];
  if (!name) {
    int i=1;
    while (i < 9999) {
      VPRINTF(itemtext, sizeof(itemtext), "Macro Item %d", i);
      if (!getChild(itemtext)) break;
      i++;
    }
    name = itemtext;
  }

  evntSingle *event = new evntSingle(name, this);
  new cfgValidateRenameParent("Macro Item Name", event);
  new cfgRemoveParent("Remove Macro Item", event);
  events.appendItem(event);
  event->initConfig(); //TODO: temporary fix
  return event;
}


void vMacroList::activate() {
  evntSingle *event = events.getFirstItem();
  while (event) {
    event->activate();
    event = events.getNextItem(event);
  }
}


//--------------------------------------------------------------


void vAddMacro::onClick(HPLUGIN source) {
  cfgButton::onClick(source);

  char itemtext[32];
  int i=1;
  while (i < 9999) {
    VPRINTF(itemtext, sizeof(itemtext), "Macro %d", i);
    if (!getParent()->getChild(itemtext)) break;
    i++;
  }

  getParent()->setExpanded(TRUE);
  new vMacroList(itemtext, getParent());
}
