/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WHK_TRACK_H_
#define _WHK_TRACK_H_


#include "whkBase.h"


class whkTrack : public whkBase {
  public:
    whkTrack(wndBase *parentwnd=NULL);
    virtual ~whkTrack();

  public:
    int getTrackWndPos();
    int getTrackWndMin();
    int getTrackWndMax();
    int getTrackWndPage();
    void setTrackWndPos(int pos);
    void setTrackWndMin(int min);
    void setTrackWndMax(int max);
    void setTrackWndMinMan(int min, int max);
    void setTrackWndPage(int page);
  
  protected:
    virtual void onTrackWnd(int pos) {}
    virtual BOOL wndHookProc(whkInfo *info);
};


#endif //_WHK_TRACK_H_
