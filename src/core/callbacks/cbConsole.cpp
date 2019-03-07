/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"
#include "cbConsole.h"


rmxList<cbCoreConsole*> cbCoreConsole::items;
rmxList<cbCoreConsole*> cbCoreConsole::stack;
cbCoreConsole *cbCoreConsole::mainconsole = NULL;


cbCoreConsole::cbCoreConsole(HPLUGIN cb_plugin, HLIST cb_list) : cbCore(cb_plugin, cb_list) {
#ifdef _DEBUG
  char str[1024];
  VPRINTF(str, sizeof(str), "New: PLUGIN=%p  LIST=%p  NAME=%s", cb_plugin, cb_list, getName());
  CONSOLE_MAIN->print("Console", str);
#endif

  if (mainconsole == NULL) mainconsole = this;
  items.appendItem(this);
}


cbCoreConsole::~cbCoreConsole() {
  items.removeItem(this);

  while (stack.hasItem(this)) {
    stack.removeItem(this);
  }

#ifdef _DEBUG
  char str[1024];
  VPRINTF(str, sizeof(str), "Delete: PLUGIN=%p  LIST=%p  NAME=%s", getRemotePlugin(), getRemoteConfig(), getName());
  CONSOLE_MAIN->print("Console", str);
#endif
}


cbCoreConsole *cbCoreConsole::getConsoleCallback(HLIST list) {
  cbCoreConsole *cb = items.getFirstItem();
  while (cb) {
    if (cb->getRemoteConfig() == list) return cb;
    cb = items.getNextItem(cb);
  }
  return NULL;
}


void cbCoreConsole::pushConsoleStack(cbCoreConsole *console) {
  if (!console) return;
  stack.appendItem(console);
}

cbCoreConsole* cbCoreConsole::peekConsoleStack() {
  return stack.getLastItem();
}

cbCoreConsole* cbCoreConsole::popConsoleStack() {
  cbCoreConsole *console = stack.getLastItem();
  stack.removeLastItem();
  return console;
}


void cbCoreConsole::console_print(const char *text, HCONSOLE console) {
  cbCoreConsole *con = getConsole(console);
  if (con) {
    con->print(text);
  }

  else if (console == CONSOLE_BROADCAST) {
    con = items.getFirstItem();
    while (con) {
      con->print(text);
      con = items.getNextItem(con);
    }
  }
}


void cbCoreConsole::console_clear(HCONSOLE console) {
  cbCoreConsole *con = getConsole(console);
  if (con) con->clear();
}


HPLUGIN cbCoreConsole::console_getPlugin(HCONSOLE console) {
  cbCoreConsole *con = getConsole(console);
  return (con) ? (con->getRemotePlugin()) : (NULL);
}


HLIST cbCoreConsole::console_getList(HCONSOLE console) {
  cbCoreConsole *con = getConsole(console);
  return (con) ? (con->getRemoteConfig()) : (NULL);
}


cbCoreConsole *cbCoreConsole::getConsole(HCONSOLE console) {
  if (console == CONSOLE_BROADCAST) return NULL;

  if (console == CONSOLE_MAIN) {
    return (items.hasItem(mainconsole)) ? (mainconsole) : (NULL);
  }

  if (console == CONSOLE_ACTIVE) {
    if (stack.hasItems()) return stack.getLastItem();
    return (items.hasItem(mainconsole)) ? (mainconsole) : (NULL);
  }

  cbCoreConsole *con = (cbCoreConsole*)console;
  return (items.hasItem(con)) ? (con) : (NULL);
}

//----------------------------------------------------------------------------------


cbRootConsole::cbRootConsole(HPLUGIN plug) : cbRoot(plug) {
  HLIST list = plug->root_getFirstChild();
  while (list) {
    if (VSTRCMP(plug->list_getType(list), "console") == 0) {
      new cbCoreConsole(plug, list);
    }
    list = plug->root_getNextChild(list);
  }
}


cbRootConsole::~cbRootConsole() {
}


void cbRootConsole::cb_onInsertChild(HLIST child, HLIST insert) {
  if (VSTRCMP(getRemotePlugin()->list_getType((HLIST)child), "console") == 0) {
    new cbCoreConsole(getRemotePlugin(), child);
  }
}
