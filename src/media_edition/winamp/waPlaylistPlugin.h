/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WAPLAYLISTPLUGIN_H_
#define _WAPLAYLISTPLUGIN_H_


#include "../pluginPlaylist.h"


class waPlaylistPlugin : public pluginPlaylist {
  public:
    waPlaylistPlugin(const char *path);
    virtual ~waPlaylistPlugin();

  public:
    virtual const char *getName() const { return "Winamp Playlist"; }

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


    //volume ranges from 0.0 (min) to 100.0 (max)
    virtual void   setVolume(double amount);
    virtual double getVolume();
};


#endif //_WAPLAYLISTPLUGIN_H_
