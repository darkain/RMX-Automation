/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __RMXWIN32_MASTEREXIT_H__
#define __RMXWIN32_MASTEREXIT_H__


#include "../../../sdk/cfg/cfgMenu.h"
#include "../../../sdk/Win32/Timer.h"


class vMasterExit : public cfgMenu/*, public vTimer*/ {
  public:
    vMasterExit(cfgBase *parent) : cfgMenu("E&xit", parent)/*, vTimer(10, FALSE)*/ {}
    virtual ~vMasterExit() {}


  public:
    virtual void onClick(HPLUGIN source) {
      //startTimer();
	  plg->destroyPassive();
      cfgMenu::onClick(source);
    }

/*
    virtual void onTimer(DWORD systemTime) {
      stopTimer();
      plg->destroy();
      vTimer::onTimer(systemTime);
    }
*/
};


#endif //__RMXWIN32_MASTEREXIT_H__
