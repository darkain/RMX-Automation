/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _GAME_AXIS_H_
#define _GAME_AXIS_H_


#include "../../sdk/evnt/evntRelative.h"
#include "Gaming.h"


class vGameAxis : public evntRelative {
  public:
    vGameAxis(GUID guid, const char *name, cfgBase *parent);
    virtual ~vGameAxis();

  public:
    inline GUID getGuid() const { return getParamGuid("guid"); }
};


#endif //_GAME_AXIS_H_
