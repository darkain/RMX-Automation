/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "funcPress.h"


funcPress::funcPress(const char *function_name, const char *script_name, cfgRoot *parent)
 : funcBase(function_name, script_name, parent) {

	setParam(   "style", "press");
	setParamInt("state", 0)->setSavable(false);
}


funcPress::funcPress(const char *function_name, const char *script_name, funcGroup *parent)
 : funcBase(function_name, script_name, parent) {

	setParam(   "style", "press");
	setParamInt("state", 0)->setSavable(false);
}


funcPress::~funcPress() {
}


void funcPress::onActivated(HPLUGIN source, HLIST input, HLIST output) {
  funcBase::onActivated(source, input, output);

  double      value = source->list_getParamNumber(input, "value");
  const char *style = source->list_getParamSafe(  input, "style");

  BOOL curState = !!getParamInt("state");
  BOOL newState = FALSE;
  
  if (!VSTRCMP(style, "press")) {
    newState = ( value > (curState ? 75.0 :  25.0) );
  } else if (!VSTRCMP(style, "absolute")) {
    newState = ( value > (curState ? 75.0 :  25.0) );
  } else if (!VSTRCMP(style, "relative")) {
    newState = ( value > (curState ? 50.0 : -50.0) );
  }
  
  if (curState != newState) {
    setParamInt("state", newState);
    onActivated(source, input, output, newState);
    if (!newState) removeFunctionCallback();
  }
}
