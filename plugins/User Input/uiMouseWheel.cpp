/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../../sdk/Win32/Win32.h"
#include "uiMouseWheel.h"


uiMouseWheel::uiMouseWheel(int iwheel, const char *function_name, const char *script_name, funcGroup *parent)
  : funcRelative(function_name, script_name, parent) {

  wheel = iwheel;
}


uiMouseWheel::~uiMouseWheel() {
}


void uiMouseWheel::onActivated(HPLUGIN source, HLIST input, HLIST output, double delta) {
  if (delta < 10  &&  delta > -10) return;  //TODO: add "deadzone" to evntRelative

  INPUT inputdata;
  ZeroMemory(&inputdata, sizeof(inputdata));

  inputdata.type           = INPUT_MOUSE;
  inputdata.mi.dwExtraInfo = GetMessageExtraInfo();
  inputdata.mi.mouseData   = static_cast<signed short>((-delta / 300) * WHEEL_DELTA);
  inputdata.mi.dwFlags     = wheel;

  SendInput(1, &inputdata, sizeof(inputdata));
}
