/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "funcToggle.h"


funcToggle::funcToggle(const char *function_name, const char *script_name, cfgRoot *parent)
 : funcPress(function_name, script_name, parent) {

	setParamInt("oldstate", 0)->setSavable(false);
}


funcToggle::funcToggle(const char *function_name, const char *script_name, funcGroup *parent)
 : funcPress(function_name, script_name, parent) {

	setParamInt("oldstate", 0)->setSavable(false);
}


funcToggle::~funcToggle() {
}


void funcToggle::setCurrentState(BOOL state) {
	setParamInt("state", !!state);
}


void funcToggle::onActivated(HPLUGIN source, HLIST input, HLIST output) {
  funcBase::onActivated(source, input, output);

  double      value = source->list_getParamNumber(input, "value");
  const char *style = source->list_getParamSafe(  input, "style");

  BOOL curState = !!getParamInt("state");
  BOOL oldState = !!getParamInt("oldstate");
  BOOL newState = FALSE;
  
  if (!VSTRCMP(style, "press")) {
    newState = ( value > (curState ? 75.0 :  25.0) );
  } else if (!VSTRCMP(style, "absolute")) {
    newState = ( value > (curState ? 75.0 :  25.0) );
  } else if (!VSTRCMP(style, "relative")) {
    newState = ( value > (curState ? 50.0 : -50.0) );
  }
  
  if (curState != oldState) {
    setParamInt("oldstate", newState);

    curState = !curState;
    setParamInt("state", curState);

    onActivated(source, input, output, curState);
  }
}
