/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cfgBool.h"


cfgBool::cfgBool(const char *initname, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : cfgBase(initname, "bool", parent, insert, paramlist) {

  setValueInt(0);
}


cfgBool::~cfgBool() {
}


void cfgBool::setValue(BOOL value, BOOL force) {
  value = !!value;
  if (force) {
    setValueInt(value);
  } else if (value != getValue()) {
    setValueInt(value);
  }
}


BOOL cfgBool::getValue() const {
  return getValueInt();
}


void cfgBool::validateUserData(prmBase *userdata) {
  userdata->setValueInt( !!userdata->getValueInt() );
}
