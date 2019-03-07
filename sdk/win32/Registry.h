/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __REGISTRY_H_
#define __REGISTRY_H_

#include "Win32.h"


class vRegistry {
  public:
    vRegistry() {}
    virtual ~vRegistry() {}

    bool regKeyString(HKEY hKey, const char *regPath, const char *regName, char *returnText);
};


#endif//__REGISTRY_H_

