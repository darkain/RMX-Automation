/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../../sdk/Win32/Win32.h"
#include "uiMouseAxis.h"
#include "uiMouse.h"


uiMouseAxis::uiMouseAxis(int iaxis, const char *function_name, const char *script_name, funcGroup *parent)
 : funcAbsolute(function_name, script_name, parent) {

   axis = iaxis;
   
   setMinMax( 0, GetSystemMetrics(axis==1 ? SM_CXSCREEN : SM_CYSCREEN) );
}


uiMouseAxis::~uiMouseAxis() {
}


void uiMouseAxis::onActivated(HPLUGIN source, HLIST input, HLIST output, double value) {
  SetCursorPos(mouse->getX(), mouse->getY());
}
