/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RLPLAYLISTPLUGIN_H_
#define _RLPLAYLISTPLUGIN_H_


#include "../pluginPlaylist.h"


struct IRLPlayback;


class rlPlaylistPlugin : public pluginPlaylist {
  public:
    rlPlaylistPlugin(const char *path);
    virtual ~rlPlaylistPlugin();

  public:
		virtual const char *getName() const { return "RadLight Playlist"; }

    virtual void play();
//    virtual void playPause();
    virtual void pause();
    virtual void stop();

//    virtual void prev(int num);
//    virtual void next(int num);

    virtual void fastForward(int ms);
    virtual void fastRewind(int ms);
    virtual void seek(int ms);

//    virtual int  getPlaybackStatus();
//    virtual UINT getPlaylistLength() const;


    virtual void volumeMute();
    virtual void   setVolume(double amount);
    virtual double getVolume();


  protected:
    IRLPlayback *getInterface();

  private:
    IRLPlayback *rlpb;
};


#endif //_RLPLAYLISTPLUGIN_H_
