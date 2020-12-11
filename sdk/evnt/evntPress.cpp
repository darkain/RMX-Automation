/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "evntPress.h"


evntPress::evntPress(const char *initname, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : evntBase(initname, parent, insert, paramlist) {

   setParam("style", "press");
}


evntPress::~evntPress() {
}


void evntPress::activatePress() {
  if (getValue() != 100.0) setValue(100.0);
  setHighlighted(TRUE);
}


void evntPress::deactivatePress() {
  setHighlighted(FALSE);
  if (getValue() != 0.0) setValue(0.0);
}


void evntPress::setPress(BOOL press) {
  if (press) {
    activatePress();
  } else {
    deactivatePress();
  }
}
