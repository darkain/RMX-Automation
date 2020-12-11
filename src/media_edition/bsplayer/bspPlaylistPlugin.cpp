/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"


#ifdef RMX_BSP


#include "bspMain.h"
#include "bspPlaylistPlugin.h"


bspPlaylistPlugin::bspPlaylistPlugin(const char *path) : pluginPlaylist(path) {
}

bspPlaylistPlugin::~bspPlaylistPlugin() {
}


void bspPlaylistPlugin::play() {
  switch (getPlaybackStatus()) {
    case 1:
      seek(0);
      break;

    case 2:
      seek(0);
      SendMessage(hMessage, WM_BSP_CMD, BSP_Pause, 0);
      break;

    default:
      SendMessage(hMessage, WM_BSP_CMD, BSP_Play, 0);
      break;
  }
}


void bspPlaylistPlugin::playPause() {
  if (getPlaybackStatus() == 0) {
    SendMessage(hMessage, WM_BSP_CMD, BSP_Play, 0);
  } else {
    SendMessage(hMessage, WM_BSP_CMD, BSP_Pause, 0);
  }
}


void bspPlaylistPlugin::pause() {
  SendMessage(hMessage, WM_BSP_CMD, BSP_Pause, 0);
}


void bspPlaylistPlugin::stop() {
  SendMessage(hMessage, WM_BSP_CMD, BSP_Stop, 0);
}


void bspPlaylistPlugin::prev(int num) {
  for (int i=0; i<num; i++) {
    SendMessage(hMessage, WM_BSP_CMD, BSP_Prev, 0);
  }
}


void bspPlaylistPlugin::next(int num) {
  for (int i=0; i<num; i++) {
    SendMessage(hMessage, WM_BSP_CMD, BSP_Next, 0);
  }
}


void bspPlaylistPlugin::fastForward(int ms) {
  int pos = SendMessage(hMessage, WM_BSP_CMD, BSP_GetMovPos, 0);
  pos += ms;
  seek(max(pos, 0));
}


void bspPlaylistPlugin::fastRewind(int ms) {
  int pos = SendMessage(hMessage, WM_BSP_CMD, BSP_GetMovPos, 0);
  int len = SendMessage(hMessage, WM_BSP_CMD, BSP_GetMovLen, 0);
  pos -= ms;
  seek(min(pos, len));
}


void bspPlaylistPlugin::seek(int ms) {
  SendMessage(hMessage, WM_BSP_CMD, BSP_Seek, ms);
}


int bspPlaylistPlugin::getPlaybackStatus() {
  switch (SendMessage(hMessage, WM_BSP_CMD, BSP_GetStatus, 0)) {
    case 1:  return 2;  //paused
    case 2:  return 1;  //playing
  }
  return 0;
}


void bspPlaylistPlugin::volumeMute() {
  SendMessage(hMessage, WM_BSP_CMD, BSP_Mute, 0);
}


void bspPlaylistPlugin::setVolume(double amount) {
  int volume = static_cast<int>( (amount / 100) * 24 );
  SendMessage(hMessage, WM_BSP_CMD, BSP_SetVol, volume);
}


double bspPlaylistPlugin::getVolume() {
  int volume = SendMessage(hMessage, WM_BSP_CMD, BSP_GetVol, 0);
  return (((static_cast<double>(volume)) / 24) * 100);
}


#endif //RMX_BSP
