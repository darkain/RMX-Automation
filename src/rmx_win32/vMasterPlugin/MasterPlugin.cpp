/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/
#if 0


#include "../../../sdk/rmxBase.h"
#include "../../../sdk/plugBase.h"
#include "../../../sdk/cfg/cfgWndHelper.h"

#include "MasterPlugin.h"
#include "MasterEnable.h"
#include "MasterTray.h"
#include "MasterExit.h"
#include "MasterWnd.h"
#include "MasterMenu.h"
#include "MasterConsole.h"

#include "../rmx_win32.h"
#include "../vSettingWnd/SettingWnd.h"

#include "../../core/Callbacks/cbConsole.h"
#include "../../core/Callbacks/cbFunction.h"


vMasterPlugin::vMasterPlugin(const char *path) : vLocalPlugin("Master", path) {
  trayIcon = NULL;
  tree     = NULL;
}


void vMasterPlugin::onInitPlugin() {
  vLocalPlugin::onInitPlugin();
/*
  cfgBase *ident = getID();
  (ident->insertParamInt("window", (int)RmxWin32->getWnd()))->setSavable(FALSE);
*/
  tree = new cfgTree("Configuration", this);

  new vMasterMenu("Enabled", this, new vMasterEnableBool(tree, this));


#ifdef _RMX_CORE_TRAY_
  trayIcon = new vMasterTray(tree, this);
  trayIcon->setValue(TRUE);
#endif //_RMX_CORE_TRAY_


#ifdef _RMX_CORE_CONFIG_
  new vMasterMenu("Configuration", this, new vMasterWndBool(tree));
#endif //_RMX_CORE_CONFIG_


#ifdef _RMX_CORE_CONSOLE_
  if (RMXOS::isWin9x()) {
    (new vMasterConsoleBool(tree, this))->setVisible(FALSE);
  } else {
    new vMasterMenu("Console", this, new vMasterConsoleBool(tree, this));
  }
#endif //_RMX_CORE_CONSOLE_


//  CONSOLE_MAIN->print("Host", ident->getParam("host"));  //TODO: reimpliment this!
//  CONSOLE_MAIN->print("Host", ident->getParam("title"));


#if defined RMX_STANDALONE  ||  defined RMX_ENGINE
  new vMasterExit(this);
#endif
}


void vMasterPlugin::onFirstLoad() {
//  rmxUtf8ToWide titlestr(plg->getMasterIdent()->getParam("title"));
	rmxUtf8ToWide titlestr(plg->getSetting("_rmx_title"));
#ifndef _RMX_MEDIA_REMOTE_
  MessageBox(0, L"This is the first time that RMX Automation has loaded.  Default configuration values will now be applied.  You may double-click on RMX's \"X\" icon in your system tray to change configuration settings.\n\nNOTE:  Some devices may be disabled by default within the RMX configuration.  Refer to the documentation for the specific plugin you are using for details on how to configure it.\n\nCheck out http://rmx.sourceforge.net/ for additional help and support on configuring RMX.", titlestr.getBuffer(), 0);
#endif //_RMX_MEDIA_REMOTE_
  vLocalPlugin::onFirstLoad();
}

/*
void vMasterPlugin::onLoadSettings() {
  vLocalPlugin::onLoadSettings();
  unlock();  //TODO: figure out what exactly we are "unlocking" here
}
*/


RMXLONG vMasterPlugin::onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
  switch (message) {
    case EVENT_ACTIVATE:
      if (isEnabled()) {
        cbCoreFunction *cb = cbCoreFunction::getFunction(source, (HLIST)param1, 2);
        if (cb) cb->activateFunction(source, param1, param2);
      }
      break;


    case EVENT_GET_NAME:
      {
        cbCoreFunction *cb = cbCoreFunction::getFunction(source, (HLIST)param1);
        if (cb) {
          cb->setEventDescription(source, param1, param2);
        } else {
          cbCoreFunction::setDefaultDescription(source, param1, param2);
        }
      }
      break;


    case EVENT_OPEN_GUI:
      vSettingWnd::createWindow(source, (HLIST)param1);
      break;

/* assigning IDs is now deprecated - IDs now auto-assigned based on SQL IDs
    case FUNCTION_START:
      {
        vPlugin *plugin = plg->getPlugin(source);
        if (plugin) plugin->assignIds();
      }
      return 0;
*/

    case FUNCTION_DEPTH:
      return static_cast<RMXLONG>(cbCoreFunction::getQueDepth());


    case MENU_MASTER:
      if (trayIcon) trayIcon->popMenu();
      break;


    case GENERIC_OPEN:
      if (tree) {
        cfgBase *list = tree->getChild("Configuration Window");
        if (list) list->setValueInt(1);
      }
      break;


    case GENERIC_CLOSE:
      if (tree) {
        cfgBase *list = tree->getChild("Configuration Window");
        if (list) list->setValueInt(0);
      }
      break;
  }

  return vLocalPlugin::onMessage(source, message, param1, param2);
}

#endif
