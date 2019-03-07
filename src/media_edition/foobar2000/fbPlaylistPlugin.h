/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _FBPLAYLISTPLUGIN_H_
#define _FBPLAYLISTPLUGIN_H_


#include "../pluginPlaylist.h"


class playlist_control;


class fbPlaylistPlugin : public pluginPlaylist {
  public:
    fbPlaylistPlugin(const char *path);
    virtual ~fbPlaylistPlugin();

    virtual const char *getName() const { return "foobar2000 Playlist"; }

    virtual void play();
    virtual void playPause();
    virtual void pause();
    virtual void stop();
    virtual void stopAfterCurrent();

    virtual void prev(int num);
    virtual void next(int num);

    virtual void fastForward(int ms);
    virtual void fastRewind(int ms);
    virtual void seek(int ms);

    virtual int  getPlaybackStatus();
    virtual UINT getPlaylistLength() const;
    virtual void setPlaylistPosition(int pos);

    virtual void volumeMute();
    virtual void   setVolume(double amount);
    virtual double getVolume();
};


#endif //_FBPLAYLISTPLUGIN_H_