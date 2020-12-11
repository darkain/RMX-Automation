/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "pluginPlaylist.h"


pluginPlaylist *plgplaylist = NULL;


pluginPlaylist::pluginPlaylist(const char *path) : vLocalPlugin("Playlist", path) {
  plgplaylist = this;
  volume = 0;
}


pluginPlaylist::~pluginPlaylist() {
  plgplaylist = NULL;
}


void pluginPlaylist::onPostInitPlugin() {
  volume = getVolume();
  vLocalPlugin::onPostInitPlugin();
}


void pluginPlaylist::randTrack() {
  setPlaylistPosition( RAND() % getPlaylistLength() );
}


void pluginPlaylist::volumeMute() {
  double vol = getVolume();
  if (vol == 0) {
    setVolume(volume);
  } else {
    volume = vol;
    setVolume(0);
  }
}



void pluginPlaylist::onSongChange() {
  //TODO:  song change messages, are we going to have those anymore?
  //we dont want to flood the main console with these messages
  //ANSWER: yes:  "on song change" is going to be a evnt object now, as well as other player events
  //TODO: so we need to add these evnt objects then to all of the media player stuffs
}

/*
  char str[1024];
  const char *title = getConfigListParam(PLAYLIST, "pos");
  if (!title) return;
  if (!*title) return;

  VPRINTF(str, sizeof(str), "\033[1;36mNow Playing: \033[37m%s\033[0m", title);
  plg->printConsole(str, CONSOLE_BROADCAST);
}
*/
