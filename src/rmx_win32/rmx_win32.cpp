/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../../sdk/Win32/Menu.h"
#include "../../sdk/Win32/Win32.h"
#include "../../sdk/Win32/manifest.h"
#include "../rmxlib/rmx_msg.h"
#include "../core/callbacks/cbConsole.h"
#include "../core/callbacks/cbFunction.h"
#include "../core/xml/xmlSettings.h"

#include <commctrl.h>

//temporary fix for things
#include "../stand_alone/resource.h"

#include "rmx_win32.h"
//#include "SettingWnd.h"
//#include "ConfigWnd.h"
//#include "ConsoleWnd.h"

#include "vMasterPlugin/MasterPlugin.h"
#include "vSettingWnd/SettingWnd.h"


vRmxWin32 *RmxWin32 = NULL;


vRmxWin32::vRmxWin32() : wndBase("RMX Automation"), vPluginLoader() {
  setParentHWnd(NULL);  //by forcing a NULL parent, it wont create a message only window, allowing us to recieve callbacks from the OS
  rmx_master = NULL;
  RmxWin32 = this;

  LoadLibrary(L"ComCtl32.dll");
//  LoadLibrary("RichEd20.dll");

  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
  InitCommonControlsEx(&icex);
}


vRmxWin32::~vRmxWin32() {
  RmxWin32 = NULL;
}


void vRmxWin32::onInit() {
  char rmxpath[MAX_PATH] = "";
  VPRINTF(rmxpath, sizeof(rmxpath), "%s\\%s", RMXOS::getPath(), RMXOS::getName());

  //create the primary plugin
  rmx_master = new vMasterPlugin(rmxpath);

  initMasterPlugin(rmx_master);


  wndBase::onInit();
  vPluginLoader::onInit();

#ifdef RMX_IPCLOADER
  //search for other instances of RM-X running on the system
  HWND wnd = NULL;

#ifdef RMX_STANDALONE
  wnd = FindWindow("RM-X General Purpose Control", NULL);
  while (wnd) {
    onRemoteRmxWindow(wnd);
    wnd = FindWindowEx(NULL, wnd, "RM-X General Purpose Control", NULL);
  }
#endif //RMX_STANDALONE

  wnd = FindWindow("RM-X Stand-Alone", NULL);
  while (wnd) {
    onRemoteRmxWindow(wnd);
    wnd = FindWindowEx(NULL, wnd, "RM-X Stand-Alone", NULL);
  }
#endif //RMX_IPCLOADER

}


void vRmxWin32::initMasterPlugin(vLocalPlugin *master) {
  //add stuff the core wants
  vPluginLoader::initMasterPlugin(master);

#if 0 //This is no longer saved with the master plugin, it is now apart of the core "setting" table
  cfgBase *ident = master->getID();
  char str[128] = "";
  VPRINTF(str, sizeof(str), "%d.%d.%d", RMXOS::getOsVersionMajor(), RMXOS::getOsVersionMinor(), RMXOS::getOsVersionBuild());
  (ident->insertParam("windows", str))->setReadOnly(TRUE);
  (ident->insertParamInt("platform", RMXOS::getOsVersionPlatform()))->setReadOnly(TRUE);

#ifdef _M_X64
  (ident->insertParam("arch", "x64"))->setReadOnly(TRUE);
#else
  (ident->insertParam("arch", "x86"))->setReadOnly(TRUE);
#endif

#endif // 0

  //prepare our plugin tree
  getPluginTree();

  //finalize initialization of master plugin
  initMasterLists(master);
  master->onInitPlugin();
  master->initConfig();


  //load master plugin settings
//  DEBUGMSG("Core", "PREPARING TO LOAD SETTINGS");
//  master->onPreLoadSettings();

//  DEBUGMSG("Core", "LOADING SETTINGS");
//  xmlSetting loadxml(master);

//  DEBUGMSG("Core", "PREPARING TO FINALIZE SETTINGS");
//  master->onLoadSettings();

//  DEBUGMSG("Core", "FINISHED LOADING SETTING");
}


void vRmxWin32::onQuit() {
  vSettingWnd::destroyAll();

  //manually kill all child windows (MESSAGE BOXES)
  EnumChildWindows(getWnd(), EnumChildProc, 0);

  vPluginLoader::onQuit();
}


void vRmxWin32::onRmxPostLoad() {
  vPluginLoader::onRmxPostLoad();
  if (!rmx_master) return;
  rmx_master->unlock();
}



void vRmxWin32::onRemoteRmxWindow(HWND hwnd, BOOL notify) {
  /*  TODO:  fix IPC
  if (hwnd == getWnd()) return;                //dont link to ourself
  if (ipcPlugin::hasWindowLink(hwnd)) return;  //dont link to windows we already know about

//#ifdef RMX_STANDALONE
  //enumerate all remote plugin objects
  BOOL master = TRUE;

  UINT id = SendMessage(hwnd, WM_USER+300, 2, 0);  //param1 1=master, 2=first, 3=next
  while (id) {
    UINT ret = SendMessage(hwnd, WM_USER+311, (UINT)getWnd(), (UINT)id);
    if (ret) {
      ipcPlugin *plugin = new ipcPlugin(hwnd, (HPLUGIN)id, ret, master);
      plg->appendPlugin(plugin);
      master = FALSE;
    }
    id = SendMessage(hwnd, WM_USER+300, 3, id);
  }
//#endif

  if (notify) SendMessage(hwnd, WM_USER+310, (UINT)getWnd(), 0);
  */
}


VUINT vRmxWin32::wndProc(UINT msg, VUINT param1, VUINT param2) {
  switch (msg) {

#if defined RMX_STANDALONE  ||  defined RMX_ENGINE
    case WM_ENDSESSION:
      destroy();
      return 1;
#endif

    case WM_USER+300:
//      switch (LOWORD(param1)) {
        //TODO: fix
//        case 1: return (UINT) getMasterPlugin()->getPluginID();
/*
        case 2: {
          vPlugin *plugin = getFirstPlugin();
          while (plugin) {
        //TODO: fix
            if (VSTRCMP(plugin->getPathRootName(), "IPC") != 0) return 0;//(UINT)plugin->getPluginID();
            plugin = getNextPlugin(plugin);
          }
        } break;
*/
/*       
        case 3: {
          vPlugin *plugin = getNextPlugin( plg->getPlugin((HPLUGIN)param2) );
          while (plugin) {
        //TODO: fix
            if (VSTRCMP(plugin->getPathRootName(), "IPC") != 0) return 0; //(UINT)plugin->getPluginID();
            plugin = getNextPlugin(plugin);
          }
        } break;
*/
        //TODO: fix
//        case 4: return (UINT) getPrevPluginHandle((HPLUGIN)param2);
//        case 5: return (UINT) getLastPluginHandle();
//        case 6: return (UINT) pluginExists((HPLUGIN)param2);
//      }
      return 0;

#ifdef RMX_IPCLOADER
    case WM_USER+301:
//      return (UINT) addIpcClient((HWND)param1, param2);
      return 0;

    case WM_USER+302:
//      return (UINT) addIpcWindow((HWND)param1);
      return 0;

    case WM_USER+303:
//      removeIpcWindow((HWND)param1);
      return 0;

    case WM_USER+310:
      onRemoteRmxWindow((HWND)param1, FALSE);
      return 0;

    case WM_USER+311: {
      if (param1 == 0) return 0;
      if (param2 == 0) return 0;
      vPlugin *plugin = plg->getPlugin((HPLUGIN)param2);
      if (!plugin) return 0;
      //do verification shit here
      return (UINT) new ipcParser((HWND)param1, plugin);
    } return 0;


    case WM_COPYDATA:
      COPYDATASTRUCT *cds = (COPYDATASTRUCT*) param2;
      if (cds->dwData == 426) return (UINT)(vTimedDelete::create((const char*)cds->lpData));
//      if (cds->dwData == 425) return onIpcMessage((BYTE*)cds->lpData, cds->cbData, (HWND)param1);
      else if (cds->dwData == 425) return vSecureParser::beginParser((BYTE*)cds->lpData, cds->cbData);
      return 0;
#endif

  }
  return wndBase::wndProc(msg, param1, param2);
}



BOOL CALLBACK vRmxWin32::EnumChildProc(HWND wnd, LPARAM lparam) {
  DestroyWindow(wnd);
  return TRUE;
}




//--------------------------------------------------------------------------------------------------------------------------

/*
saConfigWnd::saConfigWnd(vConfigList *parent) : vConfigBool("Configuration Window", parent) {
  wnd = NULL;
  locked = TRUE;

  setTooltip("Shows/Hides the main RM-X Configuration Window");

  left    = new saConfigWndPos("Left",    this);
  top     = new saConfigWndPos("Top",     this);
  width   = new saConfigWndPos("Width",   this);
  height  = new saConfigWndPos("Height",  this);
  divider = new saConfigWndPos("Divider", this);
  reset   = new saConfigWndReset(this);

  width->setDefault(750);
  width->setMin(500);

  height->setDefault(500);
  height->setMin(200);

  divider->setDefault(150);
  divider->setMinMax(100, 300);
}


saConfigWnd::~saConfigWnd() {
  if (wnd) {
    delete wnd;
    wnd = NULL;
  }
}


void saConfigWnd::showWindow() {
  if (!getValue()) setValue(TRUE);
  if (locked) return;
  if (!wnd) {
    locked = TRUE;
    int x, y, w, h, div;
    x = left->getValue();   y = top->getValue();
    w = width->getValue();  h = height->getValue();
    div = divider->getValue();
    wnd = new vConfigWnd(this);
    wnd->moveresize(x, y, w, h);
    wnd->setDivider(div);
    wnd->init();
    wnd->setName(plg->getMasterIdent()->getParam("title"));
    wnd->setIcon(plg->getIconSmall());
    locked = FALSE;
  }
  wnd->showWindow();
  wnd->setForeground();
  wnd->setFocus();
}


void saConfigWnd::hideWindow() {
  if (getValue()) setValue(FALSE);
  locked = TRUE;
  delete wnd;
  wnd = NULL;
  locked = FALSE;
}

void saConfigWnd::setX(int x) {
  if (wnd) wnd->setX(x);
}

void saConfigWnd::setY(int y) {
  if (wnd) wnd->setY(y);
}

void saConfigWnd::setWidth(int w) {
  if (wnd) wnd->setWidth(w);
}

void saConfigWnd::setHeight(int h) {
  if (wnd) wnd->setHeight(h);
}




void saConfigWnd::updatePosition() {
  if (locked) return;
  if (!wnd) return;
  locked = TRUE;
  wnd->setDivider(divider->getValue());
  wnd->moveresize(left->getValue(), top->getValue(), width->getValue(), height->getValue());
  locked = FALSE;
}


void saConfigWnd::setPosition(int x, int y) {
  if (locked) return;
  locked = TRUE;
  left->setValue(x);
  top->setValue(y);
  locked = FALSE;
}

void saConfigWnd::setSize(int w, int h) {
  if (locked) return;
  locked = TRUE;
  width->setValue(w);
  height->setValue(h);
  locked = FALSE;
}

void saConfigWnd::setPositionSize(int x, int y, int h, int w) {
  if (locked) return;
  locked = TRUE;
  left->setValue(x);
  top->setValue(y);
  width->setValue(w);
  height->setValue(h);
  locked = FALSE;
}

void saConfigWnd::setDivider(int d) {
  if (locked) return;
  locked = TRUE;
  divider->setValue(d);
  locked = FALSE;
}

void saConfigWnd::onLoadSettings() {
  vConfigBool::onLoadSettings();
  locked = FALSE;
//  if (getValue()) showWindow(); else hideWindow();
}



void saConfigWnd::onSetUserData(const vConfigParam *userdata) {
  vConfigBool::onSetUserData(userdata);
  if (!plg->isLoaded()) return;
  if (getUserDataInt()) showWindow(); else hideWindow();
}



    
//--------------------------------------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------


rmxMenuItem::rmxMenuItem(const char *name, vMenu *parent, vPlugin *plugin, HLIST list)
 : vMenu(name, parent), rmxListCallback(list, plugin) {
  setChecked(getParamInt("checked"));
}


void rmxMenuItem::cb_onSetParam(const vConfigParam *param) {
  rmxListCallback::cb_onSetParam(param);
  HPARAM hparam = (HPARAM)param;
  if (VSTRCMP(getPlugin()->param_getName(hparam), "checked") == 0) {
    setChecked(getPlugin()->param_getValueInt(hparam));
  }
}
*/
