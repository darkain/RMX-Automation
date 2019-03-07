/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CB_ROOT_H_
#define _CB_ROOT_H_


#include "cbCore.h"


class cbRoot : public cbCore {
  protected:
    cbRoot(HPLUGIN plugin) : cbCore(plugin, plugin->list_getRoot()) {}

  public:
    virtual ~cbRoot() {}
};


#endif //_CB_ROOT_H_
