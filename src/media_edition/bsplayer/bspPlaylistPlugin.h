/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _BSPPLAYLISTPLUGIN_H_
#define _BSPPLAYLISTPLUGIN_H_


#include "../pluginPlaylist.h"


class bspPlaylistPlugin : public pluginPlaylist {
  public:
    bspPlaylistPlugin(const char *path);
    virtual ~bspPlaylistPlugin();

  public:
    virtual const char *getName() const { return "BSplayer Playlist"; }

    virtual void play();
    virtual void playPause();
    virtual void pause();
    virtual void stop();

    virtual void prev(int num);
    virtual void next(int num);

    virtual void fastForward(int ms);
    virtual void fastRewind(int ms);
    virtual void seek(int ms);

    virtual int  getPlaybackStatus();

    virtual void volumeMute();
    virtual void   setVolume(double amount);
    virtual double getVolume();
};


#endif //_BSPPLAYLISTPLUGIN_H_
