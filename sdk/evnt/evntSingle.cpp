/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "evntSingle.h"


evntSingle::evntSingle(const char *initname, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : evntPress(initname, parent, insert, paramlist), vTimer(100, FALSE) {
}


void evntSingle::onActivated() {
  evntBase::onActivated();
  setHighlighted(TRUE);
  startTimer();
}


void evntSingle::onTimer(DWORD systemTime) {
  vTimer::onTimer(systemTime);
  setHighlighted(FALSE);
  stopTimer();
}


void evntSingle::activate() {
  //TODO: this will mess up "highlighted", find a way to fix it
  activatePress();
  deactivatePress();
}
