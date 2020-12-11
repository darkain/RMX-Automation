/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _HID_ENUM_H_
#define _HID_ENUM_H_


#include "../../sdk/rmxList.h"
#include "hidBase.h"


class hidEnum {
  protected:
    hidEnum();
    virtual ~hidEnum();

  protected:
    virtual hidBase *enumDevice(HANDLE devhandle, HIDD_ATTRIBUTES *devattrib, const char *path)=0;

  public:
    hidBase *getDevice(unsigned short vendor, unsigned short product) const;
    hidBase *getDevice(const char *path) const;

    void enumerate();

  private:
    rmxList<hidBase*> devices;
    GUID guid;
};


#endif //_HID_ENUM_H_