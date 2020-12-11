/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../../sdk/Win32/Win32.h"
#include "uiMouseButton.h"


uiMouseButton::uiMouseButton(int iup, int idown, int idata, const char *function_name, const char *script_name, funcGroup *parent)
 : funcPress(function_name, script_name, parent) {

   up   = iup;
   down = idown;
   data = idata;
}


uiMouseButton::~uiMouseButton() {
}


void uiMouseButton::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  INPUT inputdata;
  ZeroMemory(&inputdata, sizeof(inputdata));

  inputdata.type           = INPUT_MOUSE;
  inputdata.mi.dwExtraInfo = GetMessageExtraInfo();
  inputdata.mi.mouseData   = data;
  inputdata.mi.dwFlags     = state ? down : up;

  SendInput(1, &inputdata, sizeof(inputdata));
}