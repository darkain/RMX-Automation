/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/
#if 0


#include "../../../sdk/rmxBase.h"
#include "../../../sdk/wnd/wndDivider.h"
#include "../../../sdk/wnd/wndButton.h"

#include "../rmx_win32.h"
#include "../../core/Loader.h"
#include "../../core/Callbacks/cbConsole.h"
#include "../../stand_alone/resource.h"

#include "ConfigWnd.h"
#include "ConfigPaneWnd.h"
#include "ConfigWndTree.h"
#include "ConfigStatus.h"
#include "TreeRootCallback.h"

#include <commctrl.h>


vConfigWnd *vConfigWnd::configwnd = NULL;


vConfigWnd::vConfigWnd() : wndBase("vConfigWnd") {
  setName( "RMX Configuration" );

  pane       = NULL;
  treeWnd    = NULL;
  status     = NULL;
  title      = NULL;
  rebar      = NULL;
  item       = NULL;
  dividerwnd = NULL;

  setBackgroundBrush(GetSysColorBrush(COLOR_BTNFACE));

//  btnSaveSettings = NULL;
//  btnLoadSettings = NULL;

  divider = 130;
  dividerwidth = 5;
  oldw = 0;
  oldh = 0;
}


vConfigWnd::~vConfigWnd() {
  cbRoot *callback = rootcallbacks.getFirstItem();
  while (callback) {
    delete callback;
    rootcallbacks.removeItem(callback);
    callback = rootcallbacks.getFirstItem();
  }

  title = NULL;
}


void vConfigWnd::onInit() {
  wndBase::onInit();

  setIcon(RMX_ICON_LARGE_XP);

//  int widths[] = {1, 2};
//  status = new wndStatus(this);
  status = new wndConfigStatus(this);
  status->init();
//  status->setStatusParts(widths, 2);
//  status->setStatusText(plg->getMasterIdent()->getParam("title"), 1);
  status->setStatusText(plg->getSetting("_rmx_title"), 1);
  status->showWindow();
//  whkCommand::insertHookWnd(status);


  rebar = new wndBase(REBARCLASSNAME, this);
  rebar->setWindowStyle(WS_CLIPCHILDREN | RBS_VARHEIGHT | RBS_AUTOSIZE | CCS_NODIVIDER | CCS_NOPARENTALIGN);
  rebar->init();
  rebar->showWindow();

  REBARINFO info;
  ZeroMemory(&info, sizeof(REBARINFO));
  info.cbSize = sizeof(REBARINFO);
  info.fMask = 0;
  info.himl = (HIMAGELIST)NULL;
  rebar->sendMessage(RB_SETBARINFO, 0, (UINT)&info);

  title = new wndLabel(this);
//  title->setName("Select an item from the list to configure");
  title->appendWindowStyleEx(WS_EX_TRANSPARENT);
  title->setTextAlign(TEXTHALIGN_CENTER, TEXTVALIGN_MIDDLE);
  title->init();
  title->setFont(NULL);
  title->showWindow();
  title->setTransparent(TRUE);

  REBARBANDINFO band;
  ZeroMemory(&band, sizeof(REBARBANDINFO));
  band.cbSize = sizeof(REBARBANDINFO);
  band.fMask = RBBIM_STYLE | RBBIM_CHILD | RBBIM_CHILDSIZE;
  band.fStyle = RBBS_NOGRIPPER;
  band.hwndChild  = title->getWnd();
  band.cyMinChild = 22;
  rebar->sendMessage(RB_INSERTBAND, (UINT)-1, (UINT)&band);
/*
//  btnLoadSettings = new wndButtonResize("Load", status);
  btnLoadSettings = new wndBase("Button", status);
  btnLoadSettings->setName("Load");
  btnLoadSettings->setId(100);
  btnLoadSettings->init();
  btnLoadSettings->moveresize(0, 0, 50, 20);
  btnLoadSettings->showWindow();

  btnSaveSettings = new wndBase("Button", status);
  btnSaveSettings->setName("Save");
  btnSaveSettings->setId(101);
  btnSaveSettings->init();
  btnSaveSettings->moveresize(0, 0, 50, 20);
  btnSaveSettings->showWindow();
*/
  treeWnd = new vConfigWndTree(this);
  treeWnd->appendWindowStyle(TVS_SHOWSELALWAYS | TVS_FULLROWSELECT | TVS_SINGLEEXPAND | TVS_HASBUTTONS | TVS_TRACKSELECT);
  treeWnd->setId(305);
  treeWnd->init();
  treeWnd->showWindow();

  dividerwnd = new wndDivider(this);
  dividerwnd->init();
  dividerwnd->move(divider-dividerwidth, 0);
  dividerwnd->showWindow();

  vPlugin *plugin = plg->getFirstPlugin();
  while (plugin) {
    new vTreeRootListCallback((HPLUGIN)plugin, this);
    plugin = plg->getNextPlugin(plugin);
  }

  wndTreeItem *item = treeWnd->getFirstItem();
  if (item) {
    item->select();
  } else {
    pane = new wndForest(this);
    pane->init();
    pane->showWindow();
  }
}


void vConfigWnd::onAppendPlugin(vPlugin *plugin) {
  new vTreeRootListCallback((HPLUGIN)plugin, this);
}


void vConfigWnd::enumList(HPLUGIN plugin, HLIST list, wndTreeItem *parent) {
  const char *type = plugin->list_getType(list);

  if (VSTRCMP(type, "tree") == 0) {
    wndTreeItem *item = treeWnd->addItem(plugin->list_getName(list), parent, plugin, list);
    if (plugin->list_isExpanded(list)) item->setExpanded(TRUE);
    
    HLIST child = plugin->list_getFirstChild(list);
    while (child) {
      enumList(plugin, child, item);
      child = plugin->list_getNextChild(list, child);
    }
  }
}


void vConfigWnd::setActiveItem(vPluginTreeItem *treeitem) {
  item = treeitem;

  delete pane;

  pane = new vConfigPaneWnd(this, item->getRemotePlugin(), item->getRemoteConfig());
  pane->init();
  pane->showWindow();
  oldw = 0;
  fakeResize();
}



void vConfigWnd::onResize(int width, int height) {
  wndBase::onResize(width, height);

  if (isMinimized()) return;

  if ( (height != oldh) && (width == oldw) ) {
    status->autoposition();
    int h = height - status->getClientHeight();
    if (dividerwnd) dividerwnd->setHeight(h, FALSE);
    if (treeWnd) treeWnd->setHeight(h, FALSE);
    if (pane) pane->setHeight(h - title->getWindowHeight(), FALSE);

    RECT r;
    getClientRect(&r);
    r.top = oldh - 40 - status->getClientHeight();
    r.bottom -= status->getClientHeight();
    invalidate(&r);
    updateWindow();

    oldh = height;
    return;
  }


  oldw = width;
  oldh = height;


  if (width < 1) return;
  if (height < 1) return;
  if (!status) return;

  status->autoposition();
/*  int sbborders[] = {0, 0, 0};
  status->sendMessage(SB_GETBORDERS, 0, (LPARAM)sbborders);
  {
    int widths[] = {100+sbborders[2], width};
    status->setStatusParts(widths, 2);
  }
//  RECT sbrect;
//  status->sendMessage(SB_GETRECT, 2, (LPARAM)&sbrect);

//  status->sendMessage(SB_GETRECT, 0, (LPARAM)&sbrect);
//  if (btnLoadSettings) btnLoadSettings->moveresize(sbrect.left+(sbborders[1]>>1),    sbrect.top+(sbborders[1]>>1), 50, sbrect.bottom-(sbborders[1]<<1));
//  if (btnSaveSettings) btnSaveSettings->moveresize(sbrect.left+(sbborders[1]>>1)+50, sbrect.top+(sbborders[1]>>1), 50, sbrect.bottom-(sbborders[1]<<1));
*/
  int bottom = status->getClientHeight();
  RECT window;
  getWindowRect(&window);

  BOOL repaint = FALSE;

  divider = dividerwnd->getX() + dividerwidth;
//  saparent->setDivider(divider);

  if (treeWnd) treeWnd->moveresize(0, 0, divider-dividerwidth, height-bottom, TRUE);

  if (rebar) { rebar->moveresize(divider, 0, width-divider, 21, FALSE); title->invalidate(); }
  if (pane) pane->moveresize(divider, 21, width-divider, height-bottom-21, FALSE);
  if (dividerwnd) dividerwnd->resize(dividerwidth, height-bottom, TRUE);

  updateWindow();
}


void vConfigWnd::setTitle(const char *titletext) {
  if (title) title->setName(titletext);
}


void vConfigWnd::setDivider(int div) {
  if (div == divider) return;
  divider = div;
  if (dividerwnd) dividerwnd->setX(div);
  fakeResize();
}


void vConfigWnd::setDividerWidth(int divw) {
  if (divw == dividerwidth) return;
  dividerwidth = divw;
  fakeResize();
}

/*
BOOL vConfigWnd::onCommand(whkInfo *info) {
  if (info) if (plg) switch (LOWORD(info->param1)) {
    case 100: plg->loadSettings(); return TRUE;
    case 101: plg->saveSettings(); return TRUE;
  }
  return whkCommand::onCommand(info);
}
*/

void vConfigWnd::selectItem(vPlugin *plugin, HLIST list) {
}


wndBase *vConfigWnd::config_create(vPlugin *plugin, HLIST list) {
  if (configwnd) {
    configwnd->setForeground();
    configwnd->setFocus();
    if (plugin  &&  list) {
      configwnd->selectItem(plugin, list);
    }
  } else {
    configwnd = new vConfigWnd();
    configwnd->init();
    if (plugin  &&  list) {
      configwnd->selectItem(plugin, list);
    }
    return configwnd;
  }
  return NULL;
}


void vConfigWnd::config_destroy() {
  delete configwnd;
  configwnd = NULL;
}
#endif
