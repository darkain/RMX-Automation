/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _HID_WII_ENUM_H_
#define _HID_WII_ENUM_H_


#include "hidEnum.h"


#define WII_REMOTE_VENDOR  0x057E
#define WII_REMOTE_PRODUCT 0x0306


class hidWiiEnum : public hidEnum {
  public:
    hidWiiEnum();
    virtual ~hidWiiEnum();

  protected:
    virtual hidBase *enumDevice(HANDLE devhandle, HIDD_ATTRIBUTES *devattrib, const char *path);
};


#endif //_HID_WII_ENUM_H_
