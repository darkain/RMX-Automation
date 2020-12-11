/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_ROOT_H_
#define _CFG_ROOT_H_


#include "cfgBase.h"


class cfgRoot : public cfgBase {
  public:
    cfgRoot(const char *name);
    virtual ~cfgRoot();

  public:
    inline int getSdkVersion() const { return getParamInt("sdk"); }
    inline int getLibVersion() const { return getParamInt("lib"); }
};


#endif //_CFG_ROOT_H_
