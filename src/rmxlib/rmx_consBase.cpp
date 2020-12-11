/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#define RMX_LIB


#include "../../sdk/rmxBase.h"
#include "../../sdk/cons/consBase.h"
#include "rmx_msg.h"


void consBase::grabActiveHandle() {
  RmxNul( (VMSG::SYSTEM, 302, this) );
}

void consBase::releaseActiveHandle() {
  RmxNul( (VMSG::SYSTEM, 303, this) );
}
