/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cfgText.h"


cfgText::cfgText(const char *initname, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : cfgBase(initname, "string", parent, insert, paramlist) {
}


cfgText::~cfgText() {
}


RMXLONG cfgText::onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
  if (message == GENERIC_GAIN_FOCUS) onGainFocus(source);
  if (message == GENERIC_LOST_FOCUS) onLostFocus(source);
  return cfgBase::onMessage(source, message, param1, param2);
}
