/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"


#ifdef RMX_RADLIGHT


#include "../../../sdk/Win32/Win32.h"
#include "../playlistFunction.h"
#include "../common.h"
#include "../resource.h"
#include "rlMain.h"
#include "rlPlaylistPlugin.h"



//class: TSplashForm
//title: Splash Screen

//class: TApplication
//title: RadLight 4.0



IRLKernel  *rlPlugins::Kern = NULL;
rlPlugins *rlRmx = NULL;


HRESULT __stdcall rlPlugins::CreateInstance(LPUNKNOWN Kernel, REFIID IID, LPVOID *Obj) {
  if (!Kernel) return E_FAIL;

  UINT tick = RMXOS::getTick();
  *Obj = NULL;
  
  // query for kernel interface
  if (FAILED(Kernel->QueryInterface(IID_IRLKernel, (void **)&Kern))) return E_UNEXPECTED;

  // let's create the module
  rlRmx = new rlPlugins(Kern, RMX_INFO);
  rmx = rlRmx;
  
  // we no longer require the interface...
  Kern->Release();

  if (!rlRmx) return E_OUTOFMEMORY;

  // and check for the interface...
  HRESULT hr = rlRmx->QueryInterface(IID, Obj);
  if (FAILED(hr))  { delete rlRmx; rmx = NULL; return hr; }

  rmx_init(tick);

  return hr;
}


HRESULT __stdcall rlPlugins::DeleteInstance() {
  if (rmx) rmx->destroy();
  rmx = NULL;
  return 1;
}



STDMETHODIMP rlPlugins::DisplayPropertyPageEx(int Param) {
  plg->getMasterPlugin()->onMessage((HPLUGIN)plg->getMasterHandle(), GENERIC_OPEN, 0, 0);
  return NOERROR;
}


STDMETHODIMP rlPlugins::HidePropertyPage() {
  plg->getMasterPlugin()->onMessage((HPLUGIN)plg->getMasterHandle(), GENERIC_CLOSE, 0, 0);
  return NOERROR;
}


//-------------------------------------------------------------------------------------------------


rlPlugins::rlPlugins(IRLKernel *Kernel, TRLModuleInfo Info) : TRLBaseModule(Kernel, Info) {
  setName(APPVERSION);

  propPage = new TPropertyPageMethod(this);
  propPage->AddRef();
}


rlPlugins::~rlPlugins() {
  saveSettings();
  INTERFACE_RELEASE(propPage);
  INTERFACE_RELEASE(Kern);
}


void rlPlugins::onInit() {
  vRmxWin32::onInit();

  char rmxpath[MAX_PATH] = "";
  VPRINTF(rmxpath, sizeof(rmxpath), "%s\\%s", RMXOS::getPath(), RMXOS::getName());

  rlPlaylistPlugin *playlist = new rlPlaylistPlugin(rmxpath);
  appendPlugin(playlist);
}


void rlPlugins::initMasterLists(vLocalPlugin *master) {
  cfgBase *ident = master->getID();
  (ident->insertParamNoSave("host", "RadLight"))->setReadOnly(TRUE);
  ident->insertParamNoSave(   "title",    APPVERSION);
  ident->insertParamNoSave(   "appname",  APPNAME);
  ident->insertParamNoSave(   "version", _APP_VERSION);
  ident->insertParamIntNoSave("build",   _SDK_BUILD);


  vRmxWin32::initMasterLists(master);


  funcGroup *playback = new funcGroup("Playback", "player", master);

  
  //Playback
  new vPlaylistFunction("Play",           "play",         1, playback);
//  new vPlaylistFunction("Play/Pause",     "playpause",    2, playback);
  new vPlaylistFunction("Pause",          "pause",        3, playback);
  new vPlaylistFunction("Stop",           "stop",         4, playback);
//  new vPlaylistFunction("Previous Track", "prev",        10, playback);
//  new vPlaylistFunction("Next Track",     "next",        11, playback);
  new vPlaylistFunction("Fast Forward",   "ffwd",        20, playback);
  new vPlaylistFunction("Fast Rewind",    "frwd",        21, playback);

  new rlCommand("Toggle Crossfade",   NULL,  playback, ACTION_PLAYBACK_TOGGLE_CROSSFADE);
  new rlCommand("Cycle Audio Stream", NULL,  playback, ACTION_PLAYBACK_CYCLE_AUDIOSTREAM);

  //volume
  funcGroup *volume = new funcGroup("Volume", "volume", master);
  new vPlaylistFunction("Up",   "up",   30, volume);
  new vPlaylistFunction("Down", "down", 31, volume);
  new vPlaylistFunction("Mute", "mute", 32, volume);

//  new rlCommand("Stream Capture", playback, ACTION_PLAYBACK_STREAM_CAPTURE);
//  new rlCommand("Play Page", playback, ACTION_PLAYBACK_PLAY_PAGE);
//  new rlCommand("Open", playback, ACTION_PLAYBACK_OPEN);
//  new rlCommand("Close", playback, ACTION_PLAYBACK_CLOSE);


  int i=0;
  IRLActionList *action = NULL;
  HRESULT hr = Kern->QueryInterface(IID_IRLActionList, (void**)&action);
  if (FAILED(hr)) return;
  int cnt = action->Count();
  if (cnt < 1) { action->Release(); return; }


  funcGroup *func = new funcGroup("RadLight Actions", "rlaction", master);

  //TODO:  make these work w/ scripts
  int ccnt = action->CategoryCount();
  for (i=0; i<ccnt; i++) {
    TRLActionCategory catinfo;
    action->GetCategory(i, &catinfo);
//    char *catname = NULL;
//    UnicodeToAnsi(catinfo.wstrDefaultDescription, &catname);
    rmxToUtf8 str(catinfo.wstrDefaultDescription);
    cats.appendItem(new rlCategory(str.getBuffer(), NULL, catinfo.wCategory, func));
//    free(catname);
  }

  //TODO:  make these work w/ scripts
  for (i=0; i<cnt; i++) {
    TRLActionInfo info;
//    char *name = NULL;
    action->GetItem(i, &info);
//    UnicodeToAnsi(info.wstrDefaultDescription, &name);
    rmxToUtf8 str(info.wstrDefaultDescription);
    new rlCommand(str.getBuffer(), NULL, getCategory(info.wCategory), info.wAction);
//    free(name);
  }

  action->Release();
}


rlCategory *rlPlugins::getCategory(UINT rlid) {
  rlCategory *r = cats.getFirstItem();
  while (r) {
    if (r->getId() == rlid) return r;
    r = cats.getNextItem(r);
  }
  return NULL;
}


//-------------------------------------------------------------------------------------------------


rlCommand::rlCommand(const char *function_name, const char *script_name, funcGroup *parent, UINT id)
 : funcSingle(function_name, script_name, parent) {
  rlid = id;
}


void rlCommand::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  if (rlRmx) rlRmx->SendRLMessage(RLM_ACTION, rlid, 0);
}


//-------------------------------------------------------------------------------------------------


STDMETHODIMP DLLGetModuleCount(DWORD *dwCount) {
  *dwCount = M_COUNT;
  return NOERROR;
}


STDMETHODIMP DLLGetModuleInfo(DWORD dwModule, TRLModuleInfo *Info) {
  if (dwModule >= 1) return E_FAIL;
  *Info = RMX_INFO;
  return NOERROR;
}


STDMETHODIMP DLLGetVersion(WORD *wVersion) {
  *wVersion = W_DLL_VERSION;
  return NOERROR;
}


#endif //RMX_RADLIGHT
