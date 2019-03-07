/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"


#ifdef RMX_WMP


#include "wmpPlaylistPlugin.h"


IWMPCore     *wmpPlaylistPlugin::core     = NULL;
IWMPControls *wmpPlaylistPlugin::controls = NULL;
IWMPSettings *wmpPlaylistPlugin::settings = NULL;


wmpPlaylistPlugin::wmpPlaylistPlugin(const char *path) : pluginPlaylist(path) {
}

wmpPlaylistPlugin::~wmpPlaylistPlugin() {
  setCore(NULL);
}


void wmpPlaylistPlugin::setCore(IWMPCore *c) {
  vrelease(controls);
  vrelease(settings);
  vrelease(core);

  core = c;
  if (!core) return;
  core->AddRef();

  core->get_controls(&controls);
  if (controls) controls->AddRef();

  core->get_settings(&settings);
  if (settings) settings->AddRef();
}


void wmpPlaylistPlugin::play() {
  if (!controls) return;
  switch (getPlaybackStatus()) {
    case 1:
    case 2:
      seek(0);
      break;
  }
  controls->play();
}


void wmpPlaylistPlugin::playPause() {
  if (!controls) return;
  if (getPlaybackStatus() == 1) {
    controls->pause();
  } else {
    controls->play();
  }
}


void wmpPlaylistPlugin::pause() {
  if (!controls) return;
  if (getPlaybackStatus() == 2) {
    controls->play();
  } else {
    controls->pause();
  }
}


void wmpPlaylistPlugin::stop() {
  if (controls) controls->stop();
}


void wmpPlaylistPlugin::prev(int num) {
  if (!controls) return;
  for (int i=0; i<num; i++) controls->previous();
}


void wmpPlaylistPlugin::next(int num) {
  if (!controls) return;
  for (int i=0; i<num; i++) controls->next();
}


void wmpPlaylistPlugin::fastForward(int ms) {
  if (!controls) return;
  double pos = 0;
  controls->get_currentPosition(&pos);
  pos += ( (static_cast<double>(ms)) / 1000);
  controls->put_currentPosition(pos);
}


void wmpPlaylistPlugin::fastRewind(int ms) {
  if (!controls) return;
  double pos = 0;
  controls->get_currentPosition(&pos);
  pos -= ( (static_cast<double>(ms)) / 1000);
  if (pos < 0)  pos = 0;
  controls->put_currentPosition(pos);
}


void wmpPlaylistPlugin::seek(int ms) {
  if (!controls) return;
  double pos = (static_cast<double>(ms)) / 1000;
  controls->put_currentPosition(pos);
}


int wmpPlaylistPlugin::getPlaybackStatus() {
  if (!core) return 0;
  WMPPlayState state;
  core->get_playState(&state);
  if (state == wmppsPlaying) return 1;
  if (state == wmppsPaused) return 2;
  return 0;
}


UINT wmpPlaylistPlugin::getPlaylistLength() const {
  if (!core) return 0;
  IWMPPlaylist *playlist = NULL;
  core->get_currentPlaylist(&playlist);
  if (!playlist) return 0;

  long len = 0;
  playlist->get_count(&len);
  return (UINT)len;
}


void wmpPlaylistPlugin::setPlaylistPosition(int pos) {
  if (!core) return;
  IWMPPlaylist *playlist = NULL;
  core->get_currentPlaylist(&playlist);
  if (!playlist) return;

  int status = getPlaybackStatus();
  IWMPMedia *item = NULL;
  playlist->get_item(pos,  &item);
  controls->put_currentItem(item);
  Sleep(1);

  switch (status) {
    case 1:
      controls->play();
      break;

    case 2: 
      controls->play();
      Sleep(1);
      controls->pause();
      break;
  }
}


void wmpPlaylistPlugin::volumeMute() {
  if (!settings) return;
  VARIANT_BOOL mute = FALSE;
  settings->get_mute(&mute);
  settings->put_mute(!mute);
}


void wmpPlaylistPlugin::setVolume(double amount) {
  settings->put_volume( static_cast<long>(amount) );
}


double wmpPlaylistPlugin::getVolume() {
  long volume = 0;
  settings->get_volume(&volume);
  return (static_cast<double>(volume));
}


#endif //RMX_WMP
