/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WMPPLAYLISTPLUGIN_H_
#define _WMPPLAYLISTPLUGIN_H_


#include "../pluginPlaylist.h"
#include "wmp.h"
#include "wmpplug.h"


class wmpPlaylistPlugin : public pluginPlaylist {
  public:
    wmpPlaylistPlugin(const char *path);
    virtual ~wmpPlaylistPlugin();

  public:
    virtual const char *getName() const { return "Windows Media Player Playlist"; }

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
    virtual UINT getPlaylistLength() const;
    virtual void setPlaylistPosition(int pos);

    virtual void volumeMute();
    virtual void   setVolume(double amount);
    virtual double getVolume();


    static IWMPCore *getCore() { return core; }
    static void setCore(IWMPCore *c);

  protected:
    static IWMPCore     *core;
    static IWMPControls *controls;
    static IWMPSettings *settings;
};


#endif //_WMPPLAYLISTPLUGIN_H_
