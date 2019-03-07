/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"


#ifdef RMX_FOOBAR2000


#include "fbMain.h"
#include "fbPlaylistPlugin.h"
//#include "SDK/playback_control.h"


fbPlaylistPlugin::fbPlaylistPlugin(const char *path) : pluginPlaylist(path) {
}


fbPlaylistPlugin::~fbPlaylistPlugin() {
}


void fbPlaylistPlugin::play() {
  static_api_ptr_t<playback_control> api;
  api->play_start(playback_control::track_command_play);
}


void fbPlaylistPlugin::playPause() {
  if (getPlaybackStatus() == 0) play(); else pause();
}


void fbPlaylistPlugin::pause() {
  static_api_ptr_t<playback_control> api;
  api->pause( !api->is_paused() );
}


void fbPlaylistPlugin::stop() {
  static_api_ptr_t<playback_control> api;
  api->play_stop();
}


void fbPlaylistPlugin::stopAfterCurrent() {
  static_api_ptr_t<playback_control> api;
  api->set_stop_after_current( !api->get_stop_after_current() );
}


void fbPlaylistPlugin::prev(int num) {
  static_api_ptr_t<playback_control> api;
  int status = getPlaybackStatus();

  for (int i=0; i<num; i++) {
    api->play_start(playback_control::track_command_prev);
    Sleep(0);
  }

  switch (status) {
    case 1:           break;
    case 2:  pause(); break;
    default: stop();  break;
  }
}


void fbPlaylistPlugin::next(int num) {
  static_api_ptr_t<playback_control> api;
  int status = getPlaybackStatus();

  for (int i=0; i<num; i++) {
    api->play_start(playback_control::track_command_next);
    Sleep(0);
  }

  switch (status) {
    case 1:           break;
    case 2:  pause(); break;
    default: stop();  break;
  }
}


void fbPlaylistPlugin::fastForward(int ms) {
  static_api_ptr_t<playback_control> api;
  api->playback_seek_delta( (static_cast<double>(ms))/1000 );
}


void fbPlaylistPlugin::fastRewind(int ms) {
  static_api_ptr_t<playback_control> api;
  api->playback_seek_delta( (static_cast<double>(-ms))/1000 ); 
}


void fbPlaylistPlugin::seek(int ms) {
  static_api_ptr_t<playback_control> api;
  double pos = (static_cast<double>(ms))/1000;
  api->playback_seek(pos); 
}


int fbPlaylistPlugin::getPlaybackStatus() {
  static_api_ptr_t<playback_control> api;
  if (api->is_playing()) return 1;
  if (api->is_paused())  return 2; 
  return 0;
}


UINT fbPlaylistPlugin::getPlaylistLength() const {
//  static_api_ptr_t<playback_control> api;
//  return (UINT) api->get_count();  
  return 0;
}


void fbPlaylistPlugin::setPlaylistPosition(int pos) {
/*  static_api_ptr_t<playback_control> api;
  int status = getPlaybackStatus();

  api->play_item(pos);

  switch (status) {
    case 1:           break;
    case 2:  pause(); break;
    default: stop();  break;
  } */
}


void fbPlaylistPlugin::volumeMute() {
  static_api_ptr_t<playback_control> api;
  api->volume_mute_toggle();
}


void fbPlaylistPlugin::setVolume(double amount) {
  static_api_ptr_t<playback_control> api;
  api->set_volume( static_cast<float>(amount - 100) ); 
}


double fbPlaylistPlugin::getVolume() {
  static_api_ptr_t<playback_control> api;
  return (api->get_volume() + 100);
}


#endif //RMX_FOOBAR2000
