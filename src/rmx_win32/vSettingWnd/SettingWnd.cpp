/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/
#if 0


#include "../../../sdk/rmxBase.h"
#include "../../../sdk/Win32/Menu.h"
#include "../../../sdk/wnd/wndStatus.h"
#include "../../../sdk/wnd/wndEdit.h"
#include "../../../sdk/wnd/wndLabel.h"
#include "../../../sdk/wnd/wndDroplist.h"
#include "../../../sdk/wnd/wndForest.h"

#include <commctrl.h>

#include "SettingWnd.h"
#include "../../stand_alone/resource.h"
#include "../../core/Loader.h"
#include "../../core/LocalPlugin.h"
#include "../../core/Callbacks/cbFunction.h"
#include "../../core/Callbacks/cbConsole.h"


rmxList<vSettingWnd*> vSettingWnd::wnds;


//---------------------------------------------------------------------------------------------------------


vSettingItem::vSettingItem(HPLUGIN cb_plugin, HLIST cb_list) : cbCoreDelay(cb_plugin, cb_list) {
  item = NULL;
}

vSettingItem::~vSettingItem() {
}


void vSettingItem::setForestItem(wndForestItem *fitem) {
  item = fitem;
  if (!item) return;
  item->setName(getName());
}

void vSettingItem::onNameChange(const char *newname) {
  if (item) item->setName(newname);
}


const char *vSettingItem::getSubOptionName() {
  return getParamSafe("paramname");
}


//---------------------------------------------------------------------------------------------------------


vSettingWnd *vSettingWnd::createWindow(HPLUGIN plug, HLIST list) {
  vSettingWnd *wnd = wnds.getFirstItem();
  while (wnd) {
    if (wnd->getRemotePlugin() == plug) {
      if (wnd->getRemoteConfig() == list) {
        wnd->setForeground();
        wnd->setFocus();
        return wnd;
      }
    }
    wnd = wnds.getNextItem(wnd);
  }

  wnd = new vSettingWnd(plug, list);
  wnd->moveresize(0, 0, 400, 200);
  wnd->init();
  wnd->showWindow();
  return wnd;
}


vSettingWnd::vSettingWnd(HPLUGIN cb_plugin, HLIST cb_list) : cbCore(cb_plugin, cb_list), wndBase("vSettingWnd") {
  status    = NULL;
  name      = NULL;
  path      = NULL;
  forest    = NULL;
  btnApply  = NULL;
  btnOk     = NULL;
  btnCancel = NULL;

  wnds.appendItem(this);
}


vSettingWnd::~vSettingWnd() {
  message(GENERIC_CLOSE);
  wnds.removeItem(this);
}


void vSettingWnd::destroyAll() {
  vSettingWnd *wnd = wnds.getFirstItem();
  while (wnd) {
    wnds.removeItem(wnd);
    delete wnd;
    wnd = wnds.getFirstItem();
  }
}


void vSettingWnd::onInit() {
  wndBase::onInit();

  message(GENERIC_OPEN, (RMXLONG)getWnd());

  setName("Assign Function");
  setIcon(RMX_ICON_LARGE_XP);

  HPLUGIN plugin = getRemotePlugin();
  HLIST   list   = getRemoteConfig();

  status = new wndStatus(this);
  status->init();
//  status->setStatusText(plg->getMasterIdent()->getParam("title"));
  status->setStatusText(plg->getSetting("_rmx_title"));
  status->showWindow();
  whkCommand::insertHookWnd(status);

  btnApply = new wndBase("Button", status);
  btnApply->setName("Apply");
  btnApply->setId(401);
  btnApply->init();
  btnApply->showWindow();

  btnOk = new wndBase("Button", status);
  btnOk->setName("OK");
  btnOk->setId(402);
  btnOk->init();
  btnOk->showWindow();

  btnCancel = new wndBase("Button", status);
  btnCancel->setName("Cancel");
  btnCancel->setId(403);
  btnCancel->init();
  btnCancel->showWindow();

  name = new wndLabel(this);
  name->setName(cbCore::getName());
  name->init();
  name->setFont(NULL);
  name->showWindow();

  path = new wndLabel(this);
  path->init();
  path->showWindow();

  {
    char str[4096] = "";  //todo: change this over to a dynamic size string
//    VSTRCPY(str, plugin->getPathRootName(), sizeof(str));  //TODO: add this into the new API
    rmxList<const char*> rmxpath;

    HLIST lst = plugin->list_getParent(list);
    while (lst) {
      rmxpath.prependItem(plugin->list_getName(lst));
      lst = plugin->list_getParent(lst);
    }
    
    const char *data = rmxpath.getFirstItem();
    while (data) {
      VSTRCAT(str, " > ", sizeof(str));
      VSTRCAT(str, data,  sizeof(str));
      rmxpath.removeItem(data);
      data = rmxpath.getFirstItem();
    }

    path->setName(str);
  }

  forest = new wndForest(this);
  forest->init();
  forest->showWindow();

  vSettingDroplist *droplist = new vSettingDroplist(forest, getRemotePlugin(), getRemoteConfig());
  droplist->init();
  droplist->showWindow();
  params.appendItem(droplist);
}


void vSettingWnd::applySettings() {
  vSettingSaver *p = params.getFirstItem();
  while (p) {
    p->onSettingSaver();
    p = params.getNextItem(p);
  }
}


void vSettingWnd::onResize(int width, int height) {
  wndBase::onResize(width, height);

  RECT sbrect;
  int parts[2];
  int sbborders[] = {0, 0, 0};

  if (status) {
    status->autoposition(); 
    status->sendMessage(SB_GETBORDERS, 0, (LPARAM)sbborders);
    parts[0] = width-140;
    parts[1] = -1;
    status->setStatusParts(parts, 2);
    status->sendMessage(SB_GETRECT, 1, (LPARAM)&sbrect);
  }

  if (name) name->moveresize(0, 0, width, 17);
  if (path) path->moveresize(0,  17, width, 17);

  if (btnOk)     btnOk->    moveresize(sbrect.left+(sbborders[1]>>1),    sbrect.top+(sbborders[1]>>1), 30, sbrect.bottom-(sbborders[1]<<1));
  if (btnCancel) btnCancel->moveresize(sbrect.left+(sbborders[1]>>1)+30, sbrect.top+(sbborders[1]>>1), 50, sbrect.bottom-(sbborders[1]<<1));
  if (btnApply)  btnApply-> moveresize(sbrect.left+(sbborders[1]>>1)+80, sbrect.top+(sbborders[1]>>1), 40, sbrect.bottom-(sbborders[1]<<1));

  if (forest) {
    forest->moveresize(0, 34, width, height - status->getClientHeight() - 34, FALSE);
    forest->invalidate();
  }
}


BOOL vSettingWnd::onCommand(whkInfo *info) {
  if (info) switch (LOWORD(info->param1)) {
    case 401: applySettings(); return TRUE;
    case 402: applySettings(); //no break, this way it also closes the window
    case 403: closeWnd(TRUE); return TRUE;
  }
  return whkCommand::onCommand(info);
}


//--------------------------------------------------------------------------------------------------------


vSettingDroplist::vSettingDroplist(wndForestBase *parent, HPLUGIN vplugin, HLIST hlist) : wndForestItem(parent) {
  plugin = vplugin;
  list   = hlist;
  selected = cbCoreFunction::getFunction(vplugin, hlist);

  button = NULL;
  back = NULL;
  hook = NULL;

  setExpanded(TRUE);
  loading = TRUE;
  setName( plugin->list_getParam(list, "display") );
}


vSettingDroplist::~vSettingDroplist() {
  button = NULL;
  back = NULL;
}


void vSettingDroplist::onInit() {
  wndForestItem::onInit();
  HPLUGIN master = plg->getMasterHandle();

  hook = new vDropMenuClick(this);

  back = new wndLabel(this);
  back->appendWindowStyle(WS_BORDER);
  back->setName( plugin->list_getParam(list, "desc") );
  back->init();
  back->showWindow();
  hook->insertHookWnd(back);

  whkMouse::insertHookWnd(back);
  whkCommand::insertHookWnd(back);
  addForestHook(back);


  button = new wndSubclass("Button", back);
  button->setBackgroundBrush((HBRUSH)GetStockObject(WHITE_BRUSH));
  button->setName("V");
  button->setId(120);
  button->init();
  button->showWindow();
  whkMouse::insertHookWnd(button);
  addForestHook(button);

  updateSubs();
  updateName(selected);

  loading = FALSE;
}


void vSettingDroplist::onResize(int width, int height) {
  wndForestItem::onResize(width, height);

  if (back) if (button) {
    back->moveresize(200, 0, width-201, getCollapseHeight());
    button->moveresize(back->getClientWidth()-20, 0, 20, back->getClientHeight());
  }
}


void vSettingDroplist::popup() {
  vMenu menu;
  HPLUGIN optionplugin = NULL;

  menu.addMenuItem("[DEFAULT]", -1);


  cbCoreFunction *callback = cbCoreFunction::getFirstFunction();
  while (callback) {
    if (callback->isVisible()) {
      const char *text = callback->getFunctionName();
      if (text  &&  *text) {
        vMenu *childmenu = new vDropMenuItem(callback, &menu, this);
        childmenu->setEnabled(callback->isEnabled());
        enumList(callback, childmenu);
      }
    }
    callback = cbCoreFunction::getNextFunction(callback);
  }

  RECT r;
  back->getWindowRect(&r);

  vMenu *item = (vMenu*)menu.popup(r.left, r.bottom, getWnd());
  if ((short)item == -1) {
    selected = NULL;
    if (back) back->setName( "[DEFAULT]" );
    updateSubs();
  } else if (item) {
    item->onSelect();
  }
}


void vSettingDroplist::enumList(cbCoreFunction *cb, vMenu *menu) {
  cbCoreFunction *child = cb->getFirstChildFunction();
  while (child) {
    if (child->isVisible()) {
      const char *text = child->getFunctionName();
      if (text  &&  *text) {
        vMenu *childmenu = new vDropMenuItem(child, menu, this);
        childmenu->setEnabled(child->isEnabled());
        enumList(child, childmenu);
      }
    }
    child = cb->getNextChildFunction(child);
  }
}


void vSettingDroplist::updateName(cbCoreFunction *cb) {
  if (!cb) {
    back->setName( "[DEFAULT]" );
    return;
  }

  int len = 1;
  rmxList<const char*> names;
  while (cb) {
    const char *name = cb->getFunctionName();
    if (name) {
      len += ((int)VSTRLEN(name) + 3);
      names.appendItem(name);
    }
    cb = cb->getFunctionParent();
  }

  char *desc = (char*) malloc(len);
  *desc = NULL;

  const char *name = names.getLastItem();
  while (name) {
    VSTRCAT(desc, name, len);
    name = names.getPrevItem(name);
    if (name) VSTRCAT(desc, " > ", len);
  }

  back->setName( desc );
  free(desc);
}


BOOL vSettingDroplist::onCommand(whkInfo *info) {
  if (info  &&  button  &&  button->getId() == LOWORD(info->param1)) {
    popup();
    return TRUE;
  }

  return wndForestItem::onCommand(info);
}


void vSettingDroplist::onSelectItem(cbCoreFunction *cb) {
  if (!back) return;
  selected = cb;
  updateName(cb);
  updateSubs();
}


void vSettingDroplist::onSettingSaver() {
  //get dynamically added params
  rmxList<HPARAM> params;
  HPARAM prm = plugin->list_getParamFirst(list);
  while (prm) {
    if (plugin->param_getSecurityEx(prm) & 0x01) params.appendItem(prm);
    prm = plugin->list_getParamNext(list, prm);
  }

  //remove dynamically added params
  prm = params.getFirstItem();
  while (prm) {
    plugin->list_removeParam(list, prm);
    params.removeItem(prm);
    prm = params.getFirstItem();
  }

  if (!selected) {
    plugin->list_setParam(list, "plugin", NULL);
    plugin->list_setParam(list, "list",   NULL);
    return;
  }

  plugin->list_setParamLong(list, "plugin", (RMXLONG)selected->getRemotePlugin()->getId());
  plugin->list_setParamLong(list, "list",   (RMXLONG)selected->getRemotePlugin()->list_getParamLong(selected->getRemoteConfig(), "id"));

  //add new dynamically added params
  vSettingItem *sub = subs.getFirstItem();
  while (sub) {
    const char *value = sub->getSubOptionValue();

    if (value  &&  *value) {
      int len = (int)VSTRLEN(value);
      char *newval = (char*)malloc(len+1);
      char *val = newval;
      memcpy(newval, value, len+1);

      //remove leading whitepsace
      while (*val) {
        if (*val == ' ' || *val == '\t' || *val == 0x10 || *val == 0x13) val++; else break;
      }

      //remove prefix
      const char *prefix = sub->getParamSafe("prefix");
      if (*prefix != NULL) {
        while (*val) {
          if (*prefix == NULL) {
            *val = NULL;
            val++;
            break;
          }
          if (*val != *prefix) break;
          val++;
          prefix++;
        }
      }

      //remove trailing whitespace
      char *end = val;
      while (*end) end++;
      end--;
      while (*end) {
        if (*end == ' ' || *end == '\t' || *end == 0x10 || *end == 0x13) {
          *end = NULL;
          end--;
        } else break;
      }

      //remove suffix
      const char *suffix = sub->getParamSafe("suffix");
      if (*suffix != NULL) {
        const char *sufend = suffix;
        while (*sufend) *sufend++;
        sufend--;

        while (end > val) {
          if (sufend == suffix) {
            *end = NULL;
            break;
          }
          if (*end != *sufend) break;
          end--;
          sufend--;
        }
      }

      prm = plugin->list_insertParam(list, sub->getSubOptionName());
      plugin->param_setValue(prm, val);
      free(newval);
    }

    else {
      prm = plugin->list_insertParam(list, sub->getSubOptionName());
      plugin->param_setValue(prm, NULL);
    }

    //cheap hack since the external API disallows setting security information
    vPlugin *vplug = plg->getPlugin(plugin);
    vplug->param_setSecurityEx(prm, 0x01);
    sub = subs.getNextItem(sub);
  }
}



void vSettingDroplist::updateSubs() {
  subs.emptyItems();
  deleteChildren();

  if (!selected) return;

  cbCoreFunction *cb = cbCoreFunction::getFunction(plugin, list);

  HLIST child = selected->getFirstChild();
  while (child) {
    vSettingItem *item = addSub(selected->getRemotePlugin(), child);
    if (item) {
      char str[1024]="";
      const char *prefix = item->getParamSafe("prefix");
      const char *suffix = item->getParamSafe("suffix");

      if (selected == cb) {
        VPRINTF(str, sizeof(str), "%s%s%s", prefix, plugin->list_getParamSafe(list, item->getSubOptionName()), suffix);
      } else {
        VPRINTF(str, sizeof(str), "%s%s%s", prefix, item->getParamSafe("default"), suffix);
      }

      item->setSubOptionValue(str);
    }
    child = selected->getNextChild(child);
  }
}


vSettingItem *vSettingDroplist::addSub(HPLUGIN cb_plugin, HLIST cb_list) {
  const char *type = cb_plugin->list_getType(cb_list);
  vSettingItem *item = NULL;

  if (VSTRCMP(type, "string") == 0) {
    item = new vSettingSubEdit(cb_plugin, cb_list, this);
  } else if (VSTRCMP(type, "int") == 0) {
    item = new vSettingSubEditInt(cb_plugin, cb_list, this);
  } else if (VSTRCMP(type, "stringlist") == 0) {
    item = new vSettingSubDropList(cb_plugin, cb_list, this);
  } else if (VSTRCMP(type, "intlist") == 0) {
    item = new vSettingSubDropList(cb_plugin, cb_list, this);
  } else if (VSTRCMP(type, "editstringlist") == 0) {
    item = new vSettingSubDropListEdit(cb_plugin, cb_list, this);
  } else if (VSTRCMP(type, "editintlist") == 0) {
    item = new vSettingSubDropListEditInt(cb_plugin, cb_list, this);
  }

  if (item) {
    subs.appendItem(item);
    wndForestItem *fitem = item->getForstItem();
    if (!fitem) return item;
    fitem->init();
    fitem->showWindow();
  }
  return item;
}



//--------------------------------------------------------------------------------------------------------


vDropMenuItem::vDropMenuItem(cbCoreFunction *cb, vMenu *parent, vSettingDroplist *dlist)
 : cbCore(cb->getRemotePlugin(), cb->getRemoteConfig()), vMenu(cb->getFunctionName(), parent) {
  callback = cb;
  droplist = dlist;
}


void vDropMenuItem::onSelect() {
  droplist->onSelectItem(callback);
}




//--------------------------------------------------------------------------------------------------------


vSettingFunction::vSettingFunction(const char *name, functionitem *itemdata) : vDroplistItem(name, itemdata) {
  plugin = itemdata->optionplugin;
  option = itemdata->optionlist;
}

vSettingFunction::~vSettingFunction() {
}



//--------------------------------------------------------------------------------------------------------



vSettingSubDropList::vSettingSubDropList(HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parentobject, wndForestBase *insert)
 : wndForestItem(parentobject, insert), vSettingItem(cb_plugin, cb_list) {
  setForestItem(this);
  droplist = new wndDroplist(this);
  droplist->appendWindowStyle(WS_BORDER | CBS_DROPDOWNLIST | WS_VSCROLL);
  droplist->resize(200, 200);
  whkMouse::insertHookWnd(droplist);
}


void vSettingSubDropList::onInit() {
  wndForestItem::onInit();
  if (!droplist) return;
  droplist->init();
  droplist->showWindow();
  setCollapseHeight(droplist->getClientHeight());

  HPLUGIN plugin = getRemotePlugin();
  HLIST   list   = getRemoteConfig();

  char str[1024];
  const char *prefix = plugin->list_getParamSafe(list, "prefix");
  const char *suffix = plugin->list_getParamSafe(list, "suffix");

  HLIST child = plugin->list_getFirstChild(list);
  while (child) {
    if (VSTRCMP(plugin->list_getType(child), "ListItem") == 0) {
      VPRINTF(str, sizeof(str), "%s%s%s", prefix, plugin->list_getName(child), suffix);
      droplist->addDroplistItem(str);
    }
    child = plugin->list_getNextChild(list, child);
  }
}


void vSettingSubDropList::onResize(int width, int height) {
  wndForestItem::onResize(width, height);
  if (droplist) droplist->move(width-201, 0);
}


const char *vSettingSubDropList::getSubOptionValue() {
  if (!droplist) return NULL;
  droplist->updateNameFromWnd();
  return droplist->getName();
}


void vSettingSubDropList::setSubOptionValue(const char *value) {
  rmxUtf8ToWide string(value);
  if (droplist) droplist->sendMessage(CB_SELECTSTRING, -1, (VUINT)string.getBuffer());
}


//--------------------------------------------------------------------------------------------------------



vSettingSubDropListEdit::vSettingSubDropListEdit(HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parentobject, wndForestBase *insert)
 : wndForestItem(parentobject, insert), vSettingItem(cb_plugin, cb_list) {
  setForestItem(this);
  droplist = new wndDroplist(this);
  droplist->appendWindowStyle(WS_BORDER | CBS_DROPDOWN | WS_VSCROLL);
  droplist->resize(200, 200);
}


void vSettingSubDropListEdit::onInit() {
  wndForestItem::onInit();
  if (!droplist) return;
  droplist->init();
  droplist->showWindow();
  setCollapseHeight(droplist->getClientHeight());

  
  HPLUGIN plugin = getRemotePlugin();
  HLIST list     = getRemoteConfig();

  char str[1024];
  const char *prefix = plugin->list_getParamSafe(list, "prefix");
  const char *suffix = plugin->list_getParamSafe(list, "suffix");

  HLIST child = plugin->list_getFirstChild(list);
  while (child) {
    if (VSTRCMP(plugin->list_getType(child), "ListItem") == 0) {
      VPRINTF(str, sizeof(str), "%s%s%s", prefix, plugin->list_getName(child), suffix);
      droplist->addDroplistItem(str);
    }
    child = plugin->list_getNextChild(list, child);
  }
}


void vSettingSubDropListEdit::onResize(int width, int height) {
  wndForestItem::onResize(width, height);
  if (droplist) droplist->move(width-201, 0);
}


const char *vSettingSubDropListEdit::getSubOptionValue() {
  if (!droplist) return NULL;
  droplist->updateNameFromWnd();
  return droplist->getName();
}


void vSettingSubDropListEdit::setSubOptionValue(const char *value) {
  if (!droplist) return;
  droplist->setName(value);
}


//--------------------------------------------------------------------------------------------------------


vSettingSubDropListEditInt::vSettingSubDropListEditInt(HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parentobject, wndForestBase *insert)
 : vSettingSubDropListEdit(cb_plugin, cb_list, parentobject, insert) {
}


//--------------------------------------------------------------------------------------------------------


vSettingSubEdit::vSettingSubEdit(HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parentobject, wndForestBase *insert)
 : vForestEdit(parentobject, insert), vSettingItem(cb_plugin, cb_list) {
  setForestItem(this);
}

const char *vSettingSubEdit::getSubOptionValue() {
  return getEditText();
}

void vSettingSubEdit::setSubOptionValue(const char *value) {
  setEditText(value);
}


//--------------------------------------------------------------------------------------------------------


vSettingSubEditInt::vSettingSubEditInt(HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parentobject, wndForestBase *insert)
 : vSettingSubEdit(cb_plugin, cb_list, parentobject, insert) {
}

#endif
