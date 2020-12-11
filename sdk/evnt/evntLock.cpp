/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "evntLock.h"


evntLock::evntLock(const char *initname, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : evntSingle(initname, parent, insert, paramlist) {
  count = 0;
  setTimeout(10);
}


void evntLock::activate() {
  if (isUnlocked()) evntSingle::activate();
}


void evntLock::onActivated() {
  evntSingle::onActivated();
  lock();
  count = 0;
}


void evntLock::onTimer(DWORD systemTime) {
  unlock();
  count++;
  if (count == 10) {
    evntSingle::onTimer(systemTime);
  }
}
