/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _PLUGINPLAYLIST_H_
#define _PLUGINPLAYLIST_H_


#include "../core/LocalPlugin.h"


class pluginPlaylist;
extern pluginPlaylist *plgplaylist;


class pluginPlaylist : public vLocalPlugin {
  public:
    pluginPlaylist(const char *path);
    virtual ~pluginPlaylist();

  public:
    virtual BOOL removable() { return FALSE; }

    virtual void onPostInitPlugin();

    //this is where we start the fun of "unified"

    virtual void play() {}
    virtual void playPause() {}
    virtual void pause() {}
    virtual void stop() {}
    virtual void stopAfterCurrent() {}

    virtual void prev(int num) {}
    virtual void next(int num) {}
    void randTrack();

    virtual void fastForward(int ms) {}
    virtual void fastRewind(int ms) {}
    virtual void seek(int ms) {}

    virtual int  getPlaybackStatus() { return 0; }
    virtual UINT getPlaylistLength() const { return 0; }
    virtual void setPlaylistPosition(int pos) {}

    //volume ranges from 0.0 (min) to 100.0 (max)
    inline  void volumeUp(  double amount) { setVolume( min(getVolume()+amount, 100) ); }
    inline  void volumeDown(double amount) { setVolume( max(getVolume()-amount,   0) ); }
    virtual void volumeMute();
    virtual void   setVolume(double amount) {}
    virtual double getVolume() { return 100.0; }

    virtual void onSongChange();

  protected:
    double volume;
    mutable char buffer[1024];
};


#endif //_PLUGINPLAYLIST_H_
