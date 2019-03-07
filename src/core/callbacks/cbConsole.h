/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __CONSOLEHELPER_H__
#define __CONSOLEHELPER_H__


#include "../../../sdk/cb/cbRoot.h"
#include "cbPlugin.h"


class  cbCoreConsole : public cbCore {
  public:
    cbCoreConsole(HPLUGIN plugin, HLIST item);
    virtual ~cbCoreConsole();


  public:
    inline void print(const char *s) { message(CONSOLE_PRINT, (RMXLONG)s); }
    inline void clear()              { message(CONSOLE_CLEAR);             }


  public:
    inline static cbCoreConsole *getFirstCallback() { return items.getFirstItem(); }
    inline static cbCoreConsole *getLastCallback()  { return items.getLastItem();  }
    inline static cbCoreConsole *getPrevCallback(cbCoreConsole *c) { return items.getPrevItem(c); }
    inline static cbCoreConsole *getNextCallback(cbCoreConsole *c) { return items.getNextItem(c); }
    static cbCoreConsole *getConsoleCallback(HLIST list);
    
    static void pushConsoleStack(cbCoreConsole *console);
    static cbCoreConsole* peekConsoleStack();
    static cbCoreConsole* popConsoleStack();

    static void console_print(const char *text, HCONSOLE console=CONSOLE_ACTIVE);
    static void console_clear(HCONSOLE console);

    static HPLUGIN console_getPlugin(HCONSOLE console);
    static HLIST   console_getList(HCONSOLE console);

    static cbCoreConsole *getConsole(HCONSOLE console);

  private:
    static rmxList<cbCoreConsole*> items;
    static rmxList<cbCoreConsole*> stack;
    static cbCoreConsole *mainconsole;
};


class cbRootConsole : public cbRoot  {
  public:
    cbRootConsole(HPLUGIN plug);
    virtual ~cbRootConsole();
    virtual void cb_onInsertChild(HLIST child, HLIST insert);
};


class vConsolePluginCallback : public vPluginCallback {
  public:
    vConsolePluginCallback() {}
    virtual ~vConsolePluginCallback() {}
    virtual void onAppendPlugin(vPlugin *plugin) { new cbRootConsole((HPLUGIN)plugin); }
};


#endif //__CONSOLEHELPER_H__
