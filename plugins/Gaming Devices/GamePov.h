/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _GAME_POV_H_
#define _GAME_POV_H_


#include "../../sdk/evnt/evntRelative.h"
#include "Gaming.h"


class vGamePov : public cfgNull {
  public:
    vGamePov(GUID povguid, const char *name, cfgBase *parent);
    virtual ~vGamePov();

  public:
    inline GUID getGuid() const { return getParamGuid("guid"); }

    void setValue(int val);

  private:
    int value;

    evntRelative *x;
    evntRelative *y;
};


#endif //_GAME_POV_H_
