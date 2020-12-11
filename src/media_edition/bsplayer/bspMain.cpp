/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"


#ifdef RMX_BSP


#include "../common.h"
#include "../playlistFunction.h"
#include "bspMain.h"
#include "bspPlaylistPlugin.h"


PpluginInfo pInfo;
HWND hMessage = NULL;


int bspInit() {
  UINT tick = RMXOS::getTick();
  rmx = new bsPlugins();
  rmx_init(tick);
  return 0;
}


//-------------------------------------------------------------------------------------------------------


bsPlugins::bsPlugins() {
  setName(APPVERSION);
}


bsPlugins::~bsPlugins() {
}


void bsPlugins::onInit() {
  vRmxWin32::onInit();

  char rmxpath[MAX_PATH] = "";
  VPRINTF(rmxpath, sizeof(rmxpath), "%s\\%s", RMXOS::getPath(), RMXOS::getName());

  bspPlaylistPlugin *playlist = new bspPlaylistPlugin(rmxpath);
  appendPlugin(playlist);
}


void bsPlugins::initMasterLists(vLocalPlugin *master) {
  cfgBase *ident = master->getID();
  (ident->insertParam("host", "BSplayer"))->setReadOnly(TRUE);
  ident->insertParamNoSave(   "title",    APPVERSION);
  ident->insertParamNoSave(   "appname",  APPNAME);
  ident->insertParamNoSave(   "version", _APP_VERSION);
  ident->insertParamIntNoSave("build",   _SDK_BUILD);


  vRmxWin32::initMasterLists(master);


  funcGroup *playback = new funcGroup("Playback", "player", master);

  //Playback
  new vPlaylistFunction("Play",           "play",          1,            playback);
  new vPlaylistFunction("Play/Pause",     "playpause",     2,            playback);
  new vPlaylistFunction("Pause",          "pause",         3,            playback);
  new vPlaylistFunction("Stop",           "stop",          4,            playback);
  new vPlaylistFunction("Previous Track", "prev",         10,            playback);
  new vPlaylistFunction("Next Track",     "next",         11,            playback);
  new bsCommand("Previous Chapter",       "prevchap",     BSP_PrevCh,    playback);
  new bsCommand("Next Chapter",           "nextchap",     BSP_NextCh,    playback);
  new bsCommand("Previous CD",            "prevcd",       BSP_PrevCD,    playback);
  new bsCommand("Next CD",                "nextcd",       BSP_NextCD,    playback);
  new vPlaylistFunction("Fast Forward",   "ffwd",         20,            playback);
  new vPlaylistFunction("Fast Rewind",    "frwd",         21,            playback);
  new bsCommand("Jump Backward",          "jumpbackward", BSP_JumpB,     playback);
  new bsCommand("Jump Forward",           "jumpforward",  BSP_JumpF,     playback);
  new bsCommand("Toggle Full Screen",     "fullscreen",   BSP_FS_Switch, playback);

  //volume
  funcGroup *volume = new funcGroup("Volume", "volume", master);
  new vPlaylistFunction("Up",   "up",   30, volume);
  new vPlaylistFunction("Down", "down", 31, volume);
  new vPlaylistFunction("Mute", "mute", 32, volume);

  new bsCommand("Cycle Audio Stream", "cycleaudio",    BSP_CycleAS,  master);
  new bsCommand("Cycle Subtitles",    "cyclesubtitle", BSP_CycleSub, master);
  new bsCommand("Cycle Video",        "cyclevideo",    BSP_CycleVid, master);
  new bsCommand("Exit BSplayer",      "close",         BSP_Exit,     master);
}


//-------------------------------------------------------------------------------------------------------


int __stdcall CallbackProc(const DWORD cmdID, void *param1, void *param2) {
  switch (cmdID) {
    case EV_UNLOAD: rmx_quit();   break;
    case EV_CONFIG: rmx_config(); break;
  }
  return 0;
}


#ifdef __cplusplus
extern "C" {
#endif

BOOL __stdcall pluginInit(PpluginInfo plgInfo) {
  VSTRCPY(plgInfo->description, APPNAME, sizeof(plgInfo->description));
  plgInfo->plgCallback = CallbackProc;
  pInfo = plgInfo;
  hMessage = pInfo->hwndParent;
  bspInit();
  return TRUE;
}

#ifdef __cplusplus
}
#endif


#endif //RMX_BSP
