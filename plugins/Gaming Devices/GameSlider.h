/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _GAME_SLIDER_H_
#define _GAME_SLIDER_H_


#include "../../sdk/evnt/evntAbsolute.h"
#include "Gaming.h"


class vGameSlider : public evntAbsolute {
  public:
    vGameSlider(GUID guid, const char *name, cfgBase *parent);
    virtual ~vGameSlider();

  public:
    inline GUID getGuid() const { return getParamGuid("guid"); }
};


#endif //_GAME_SLIDER_H_
