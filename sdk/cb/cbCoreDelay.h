/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CB_CORE_DELAY_H_
#define _CB_CORE_DELAY_H_


#include "cbCore.h"
#include "../Win32/Timer.h"


class cbCoreDelay : public cbCore, private vTimedCallback {
  protected:
    cbCoreDelay(HPLUGIN plugin, HLIST item) : cbCore(plugin, item) {}
    virtual ~cbCoreDelay() {}

  protected:
    virtual void cb_onDelete() {
      setRemoteConfig(NULL);
      addEvent(this);
    }

    virtual void onEvent(void *event) {
      if (event == this) delete this;
    }
};


#endif //_CB_CORE_DELAY_H_
