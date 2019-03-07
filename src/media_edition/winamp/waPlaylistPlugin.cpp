/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"


#ifdef RMX_WINAMP


#include "waPlaylistPlugin.h"
#include "waMain.h"
#include "wa_ipc.h"


/*
211 Retrieves (and returns a pointer in 'ret') a string that contains the filename of a playlist entry (indexed by 'data'). Returns NULL if error, or if 'data' is out of range. 
212 Retrieves (and returns a pointer in 'ret') a string that contains the title of a playlist entry (indexed by 'data'). Returns NULL if error, or if 'data' is out of range. 
245 Sets the current playlist item 
*/

waPlaylistPlugin::waPlaylistPlugin(const char *path) : pluginPlaylist(path) {
}


waPlaylistPlugin::~waPlaylistPlugin() {
}


void waPlaylistPlugin::play() {
  switch (SendMessage(waRmx->getWinamp(), WM_WA_IPC, 0, IPC_ISPLAYING)) {
    case 1:
      SendMessage(waRmx->getWinamp(), WM_COMMAND, 40045, 0);
      break;

    case 3:
      SendMessage(waRmx->getWinamp(), WM_WA_IPC, 0, IPC_JUMPTOTIME);
      SendMessage(waRmx->getWinamp(), WM_COMMAND, 40046, 0);
      break;

    default:
      SendMessage(waRmx->getWinamp(), WM_COMMAND, 40045, 0);
  }
}


void waPlaylistPlugin::playPause() {
  switch (SendMessage(waRmx->getWinamp(), WM_WA_IPC, 0, IPC_ISPLAYING)) {
    case 1:
    case 3:
      SendMessage(waRmx->getWinamp(), WM_COMMAND, 40046, 0);
      break;

    default:
      SendMessage(waRmx->getWinamp(), WM_COMMAND, 40045, 0);
  }
}


void waPlaylistPlugin::pause() {
  SendMessage(waRmx->getWinamp(), WM_COMMAND, 40046, 0);
//  if (getPlaybackStatus() == 0) play();
//  else pause();
}


void waPlaylistPlugin::stop() {
  SendMessage(waRmx->getWinamp(), WM_COMMAND, 40047, 0);
}


void waPlaylistPlugin::stopAfterCurrent() {
  SendMessage(waRmx->getWinamp(), WM_COMMAND, 40157, 0);
}


void waPlaylistPlugin::prev(int num) {
  for (int i=0; i<num; i++) SendMessage(waRmx->getWinamp(), WM_COMMAND, 40044, 0);
}


void waPlaylistPlugin::next(int num) {
  for (int i=0; i<num; i++) SendMessage(waRmx->getWinamp(), WM_COMMAND, 40048, 0);
}


void waPlaylistPlugin::fastForward(int ms) {
  int pos = SendMessage(waRmx->getWinamp(), WM_USER, 0, 105) + ms;
  SendMessage(waRmx->getWinamp(), WM_USER, pos, 106);
}


void waPlaylistPlugin::fastRewind(int ms) {
  int pos = SendMessage(waRmx->getWinamp(), WM_USER, 0, 105) - ms;
  pos = max(pos, 0);
  SendMessage(waRmx->getWinamp(), WM_USER, pos, 106);
}


void waPlaylistPlugin::seek(int ms) {
  SendMessage(waRmx->getWinamp(), WM_USER, ms, 106);
}


int waPlaylistPlugin::getPlaybackStatus() {
  switch (SendMessage(waRmx->getWinamp(), WM_USER, 0, 104)) {
    case 1:  return 1;  //playing
    case 3:  return 2;  //paused
  }
  return 0;
}


UINT waPlaylistPlugin::getPlaylistLength() const {  
  if (!rmx) return 0;
  //todo: we should cache this info, and hook the playlist
  return SendMessage(waRmx->getWinamp(), WM_WA_IPC, 0, IPC_GETLISTLENGTH);
}


void waPlaylistPlugin::setPlaylistPosition(int pos) {
  int status = getPlaybackStatus();
  SendMessage(waRmx->getWinamp(), WM_WA_IPC, pos, IPC_SETPLAYLISTPOS);
  if (status == 1) {
    SendMessage(waRmx->getWinamp(), WM_COMMAND, 40045, 0);
  } else if (status == 2) {
    SendMessage(waRmx->getWinamp(), WM_COMMAND, 40045, 0);
    SendMessage(waRmx->getWinamp(), WM_COMMAND, 40046, 0);
  }
}


void waPlaylistPlugin::setVolume(double amount) {
  int volume = static_cast<int>( (amount/100)*255 );
  SendMessage(waRmx->getWinamp(), WM_WA_IPC, volume, IPC_SETVOLUME);
}


double waPlaylistPlugin::getVolume() {
  int volume = SendMessage(waRmx->getWinamp(), WM_WA_IPC, -(666), IPC_SETVOLUME);
  return (((static_cast<double>(volume))/255)*100);
}


#endif //RMX_WINAMP
