/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"


#ifdef RMX_RADLIGHT


#include "rlPlaylistPlugin.h"
#include "rlMain.h"


rlPlaylistPlugin::rlPlaylistPlugin(const char *path) : pluginPlaylist(path) {
  rlpb = NULL;
}

rlPlaylistPlugin::~rlPlaylistPlugin() {
  if (rlpb) rlpb->Release();
}


IRLPlayback *rlPlaylistPlugin::getInterface() {
  if (rlpb) return rlpb;
  GetModuleInterface(rlRmx->getKernal(), IID_IRLPlayback, (void**)&rlpb);
  return rlpb;
}


void rlPlaylistPlugin::play() {
  if (!getInterface()) return;
  rlpb->Play();
}

/*
void rlPlaylistPlugin::playPause() {
  if (!getInterface()) return;
}
*/

void rlPlaylistPlugin::pause() {
  if (!getInterface()) return;
  rlpb->Pause();
}


void rlPlaylistPlugin::stop() {
  if (!getInterface()) return;
  rlpb->Stop();
}

/*
void rlPlaylistPlugin::prev(int num) {
  if (!getInterface()) return;
//  rlpb->`
}


void rlPlaylistPlugin::next(int num) {
}
*/

void rlPlaylistPlugin::fastForward(int ms) {
  if (!getInterface()) return;
  double pos = 0;
  rlpb->GetCurrentPosition(&pos);
  pos += ((static_cast<double>(ms)) / 1000);
  rlpb->SetCurrentPosition(pos);
}


void rlPlaylistPlugin::fastRewind(int ms) {
  if (!getInterface()) return;
  double pos = 0;
  rlpb->GetCurrentPosition(&pos);
  pos -= ((static_cast<double>(ms)) / 1000);
  rlpb->SetCurrentPosition(pos);
}


void rlPlaylistPlugin::seek(int ms) {
  if (!getInterface()) return;
  double pos = ((static_cast<double>(ms)) / 1000);
  rlpb->SetCurrentPosition(pos);
}

/*
int rlPlaylistPlugin::getPlaybackStatus() {
  return 0;
}


UINT rlPlaylistPlugin::getPlaylistLength() const {
  return 0;
}
*/


void rlPlaylistPlugin::volumeMute() {
  if (!getInterface()) return;
  BOOL mute = FALSE;
  rlpb->GetMute( &mute );
  rlpb->SetMute( !mute );
}


void rlPlaylistPlugin::setVolume(double amount) {
  if (!getInterface()) return;
  rlpb->SetVolume(amount / 100);
}


double rlPlaylistPlugin::getVolume() {
  if (!getInterface()) return 100;
  double volume = 0;
  rlpb->GetVolume(&volume);
  return (volume * 100);
}


#endif //RMX_RADLIGHT
