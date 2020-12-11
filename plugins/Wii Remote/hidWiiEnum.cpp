/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "hidWiiEnum.h"
#include "hidWiiRemote.h"


hidWiiEnum::hidWiiEnum() {
}


hidWiiEnum::~hidWiiEnum() {
}


hidBase *hidWiiEnum::enumDevice(HANDLE devhandle, HIDD_ATTRIBUTES *devattrib, const char *path) {
  if (devattrib->VendorID == WII_REMOTE_VENDOR) {
    if (devattrib->ProductID == WII_REMOTE_PRODUCT) {
      return new hidWiiRemote(devhandle, devattrib, path);
    }
  }
  return NULL;
}
