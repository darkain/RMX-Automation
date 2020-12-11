/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/
#if 0


#include "../../../sdk/rmxBase.h"
#include "../../stand_alone/resource.h"
#include "../../core/Loader.h"
#include "../../core/LocalPlugin.h"
#include "MasterTray.h"


vMasterTray::vMasterTray(cfgBase *parent, cfgBase *watch)
 : cfgBool("Tray Icon", parent), /*cbBase(watch),*/ vTrayIcon(NULL) {

  appendItem(watch);

  icon[0] = (HICON)LoadImage(RMXOS::getInstance(), MAKEINTRESOURCE(RMX_ICON_UNINSTALL_XP), IMAGE_ICON, 16, 16, 0);
  icon[1] = (HICON)LoadImage(RMXOS::getInstance(), MAKEINTRESOURCE(RMX_ICON_LARGE_XP),     IMAGE_ICON, 16, 16, 0);
  setIcon(icon[1]);

  vTrayIcon::setName( plg->getSetting("_rmx_title") );
//  setTooltip("Shows/Hides the RMX System Tray Icon");
  init();
}


vMasterTray::~vMasterTray() {
}


//void vMasterTray::cb_onSetEnabled(BOOL enabled) {
void vMasterTray::cb_cfgMessage(cbMessage message, cfgBase *item, RMXLONG data) {
  if (message == CB_SET_ENABLED) {
    setIcon(icon[ (data) ? (1) : (0) ], FALSE);
  }
  cfgBool::cb_cfgMessage(message, item, data);
//  cbBase::cb_onSetEnabled(enabled);
}


//void vMasterTray::onSetUserData(const prmBase *userdata) {
void vMasterTray::cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert) {
  cfgBool::cb_cfgMessage(message, item, param, insert);
  if (message == CB_PARAM_UPDATED  &&  param == this) {
    vTrayIcon::setVisible(getValue());
  }
}


void vMasterTray::onRightClick(int x, int y) {
  vTrayIcon::onRightClick(x, y);
  popMenu();
}


void vMasterTray::popMenu() {
  vMenu menu;

  vPlugin *plugin = plg->getFirstPlugin();
  while (plugin) {
    HLIST list = plugin->root_getFirstChild();
    while (list) {
      if (VSTRCMP(plugin->list_getType(list), "menu") == 0) {
        recurse(&menu, (HPLUGIN)plugin, list);
      }
      list = plugin->root_getNextChild(list);
    }
    plugin = plg->getNextPlugin(plugin);
  }

  vMenu *ret = (vMenu*)menu.popup(getWnd());
  if (ret) (static_cast<vMasterTrayItem*>(ret))->onClick();
}


void vMasterTray::recurse(vMenu *menu, HPLUGIN plugin, HLIST list) {
  const char *name = plugin->list_getName(list);
  if (!name) return;
  if (!*name) return;

  if (VSTRCMP(name, "-") == 0) {
    new vMenuSep(menu);
    return;
  }
  vMenu *menuitem = new vMasterTrayItem(name, menu, plugin, list);

  HLIST child = plugin->list_getFirstChild(list);
  while (child) {
    if (VSTRCMP(plugin->list_getType(list), "menu") == 0) {
      recurse(menuitem, plugin, child);
    }
    child = plugin->list_getNextChild(list, child);
  }
}


void vMasterTray::onLeftDoubleClick(int x, int y) {
  vTrayIcon::onLeftDoubleClick(x, y);
  plg->getMasterPlugin()->onMessage((HPLUGIN)plg->getMasterHandle(), GENERIC_OPEN, 0, 0);
}


RMXLONG vMasterTray::onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
  if (message == GENERIC_OPEN) {
    popMenu();
  }
  return cfgBool::onMessage(source, message, param1, param2);
}

//-------------------------------------------------------------------------------


vMasterTrayItem::vMasterTrayItem(const char *name, vMenu *parent, HPLUGIN cb_plugin, HLIST cb_list)
 : vMenu(name, parent), cbCore(cb_plugin, cb_list) {
  setChecked(getParamInt("checked"));
}


vMasterTrayItem::~vMasterTrayItem() {
}


void vMasterTrayItem::onClick() {
  message(GENERIC_CLICK);
}


//void vMasterTrayItem::cb_onSetParam(HPARAM param) {
void vMasterTrayItem::cb_cfgMessage(cbMessage message, HPARAM param, HPARAM insert) {
  cbCore::cb_cfgMessage(message, param, insert);
  if (message == CB_PARAM_UPDATED) {
    if (VSTRCMP(getRemotePlugin()->param_getName(param), "checked") == 0) {
      setChecked(getRemotePlugin()->param_getValueInt(param));
    }
  }
}

#endif
