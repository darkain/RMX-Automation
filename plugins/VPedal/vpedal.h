/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _vpedal_h_
#define _vpedal_h_


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/evnt/evntPress.h"
#include "../../sdk/Win32/Timer.h"


class vPedal : public plugBase {
  public:
    vPedal();

  protected:
    virtual void onInit();
};


class vPedalTree : public cfgTree, public vTimer {
  public:
    vPedalTree(const char *name, cfgBase *parent);
    virtual ~vPedalTree();

  protected:
    virtual void onTimer(DWORD systemTime);

  private:
    int        button;
    evntPress *play;
    evntPress *backward;
    evntPress *forward;
};


#endif //_vpedal_h_
