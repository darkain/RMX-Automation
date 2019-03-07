/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "consBase.h"


consBase::consBase(const char *name, cfgBase *parent, HPARAMLIST paramlist)
 : cfgBase(name, "console", parent, NULL, paramlist) {

  setSavable(FALSE);
}


consBase::~consBase() {
}


RMXLONG consBase::onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
  if (message == CONSOLE_CLEAR) clear();
  if (message == CONSOLE_PRINT) print((const char*)param1);
  return cfgBase::onMessage(source, message, param1, param2);
}
