/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"


#ifdef RMX_WMP


#include "../../../sdk/Win32/Win32.h"

#include <atlbase.h>
CComModule _Module;
#include <atlcom.h>

#include "wmp.h"
#include "wmpplug.h"
#include "wmpMain.h"
#include "wmpPlaylistPlugin.h"
#include "../playlistFunction.h"


#include <atlbase.h>


wmpPlaylistPlugin *playlist = NULL;


STDAPI DllRegisterServer(void) {
  HRESULT hr = _Module.RegisterServer();
  WMPNotifyPluginAddRemove();
  return hr;
}


STDAPI DllUnregisterServer(void) {
  HRESULT hr = _Module.UnregisterServer();
  WMPNotifyPluginAddRemove();
  return hr;
}


//---------------------------------------------------------


CwmpRmx::CwmpRmx() {
}

CwmpRmx::~CwmpRmx() {
}


HRESULT CwmpRmx::FinalConstruct() {
  return S_OK;
}


void CwmpRmx::FinalRelease() {
  rmx_quit();
  ReleaseCore();
}


HRESULT CwmpRmx::SetCore(IWMPCore *pCore) {
  HRESULT hr = S_OK;
  ReleaseCore();
  if (pCore == NULL) {
    return S_OK;
  }
  m_spCore = pCore;
  wmpPlaylistPlugin::setCore(pCore);

  if (!rmx) {
    UINT tick = RMXOS::getTick();
    rmx = new wmpPlugins();
    rmx_init(tick);
  }

  return hr;
}


void CwmpRmx::ReleaseCore() {
  m_spCore = NULL;
}


HRESULT CwmpRmx::Create(HWND hwndParent, HWND *phwndWindow) {
  return S_OK;
}



HRESULT CwmpRmx::Destroy() {
  return S_OK;
}


HRESULT CwmpRmx::TranslateAccelerator(LPMSG lpmsg) {
  return S_OK;
}


HRESULT CwmpRmx::DisplayPropertyPage(HWND hwndParent) {
  if (plg) {
    plg->getMasterPlugin()->onMessage((HPLUGIN)plg->getMasterHandle(), GENERIC_OPEN, 0, 0);
    return S_OK;
  }
  return E_NOTIMPL;
}


HRESULT CwmpRmx::GetProperty(const WCHAR *pwszName, VARIANT *pvarProperty) {
  return E_NOTIMPL;
}


HRESULT CwmpRmx::SetProperty(const WCHAR *pwszName, const VARIANT *pvarProperty) {
  return E_NOTIMPL;
}


//-------------------------------------------------------------------------------------


wmpPlugins::wmpPlugins() {
	setName(APPVERSION);
}


wmpPlugins::~wmpPlugins() {
}


void wmpPlugins::onInit() {
  vRmxWin32::onInit();

  char rmxpath[MAX_PATH] = "";
  VPRINTF(rmxpath, sizeof(rmxpath), "%s\\%s", RMXOS::getPath(), RMXOS::getName());

  playlist = new wmpPlaylistPlugin(rmxpath);
  appendPlugin(playlist);
}


void wmpPlugins::initMasterLists(vLocalPlugin *master) {
  cfgBase *ident = master->getID();
  (ident->insertParam("host", "Windows Media Player"))->setReadOnly(TRUE);
  ident->insertParamNoSave(   "title",    APPVERSION);
  ident->insertParamNoSave(   "appname",  APPNAME);
  ident->insertParamNoSave(   "version", _APP_VERSION);
  ident->insertParamIntNoSave("build",   _SDK_BUILD);


  vRmxWin32::initMasterLists(master);

  
  funcGroup *playback = new funcGroup("Playback", "player", master);

  new vPlaylistFunction("Play",           "play",         1, playback);
  new vPlaylistFunction("Play/Pause",     "playpause",    2, playback);
  new vPlaylistFunction("Pause",          "pause",        3, playback);
  new vPlaylistFunction("Stop",           "stop",         4, playback);
  new vPlaylistFunction("Previous Track", "prev",        10, playback);
  new vPlaylistFunction("Next Track",     "next",        11, playback);
  new vPlaylistFunction("Random Track",   "randtrack",   12, playback);
  new vPlaylistFunction("Fast Forward",   "ffwd",        20, playback);
  new vPlaylistFunction("Fast Rewind",    "frwd",        21, playback);


  //Volume
  funcGroup *volume = new funcGroup("Volume", "volume", master);
  new vPlaylistFunction("Up",   "up",   30, volume);
  new vPlaylistFunction("Down", "down", 31, volume);
  new vPlaylistFunction("Mute", "mute", 32, volume);
}


const char *wmpPlugins::getHostName() {
  IWMPCore *core = wmpPlaylistPlugin::getCore();
  if (!core) return "No Core";

  BSTR ustr = NULL;
//  char *astr = NULL;

  core->get_versionInfo(&ustr);
//  UnicodeToAnsi(ustr, &astr);
  rmxToUtf8 unistr(ustr);

  char str[128];
  VPRINTF(str, sizeof(str), "Microsoft Windows Media Player (WMP) - v%s", unistr.getBuffer());

  return vTimedDelete::create(str);
}


//-------------------------------------------------------------------------------------


wmpFunction::wmpFunction(const char *function_name, const char *script_name, funcGroup *parent, int cmd)
: funcSingle(function_name, script_name, parent) {
  command = cmd;
}


wmpFunction::~wmpFunction() {
}


void wmpFunction::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  IWMPCore *core = wmpPlaylistPlugin::getCore();
  if (!core) return;

  IWMPControls *controls = NULL;
  core->get_controls(&controls);
  if (!controls) return;

  switch (command) {
    case 1: controls->play();     break;
    case 2: controls->pause();    break;
    case 3: controls->stop();     break;
    case 4: controls->previous(); break;
    case 5: controls->next();     break;
  }
}


#endif
