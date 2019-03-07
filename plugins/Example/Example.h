/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _EXAMPLE_H_
#define _EXAMPLE_H_


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/config.h"


class plugExample : public plugBase {
  public:
    plugExample();

  protected:
    virtual void onInit();
};


#endif //_EXAMPLE_H_
