/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "funcSingle.h"


funcSingle::funcSingle(const char *function_name, const char *script_name, cfgRoot *parent)
 : funcPress(function_name, script_name, parent) {
}


funcSingle::funcSingle(const char *function_name, const char *script_name, funcGroup *parent)
 : funcPress(function_name, script_name, parent) {
}


funcSingle::~funcSingle() {
}


void funcSingle::onActivated(HPLUGIN source, HLIST input, HLIST output) {
  //funcBase::onActivated(source, input, output);
  //do NOT call up the hirarchy here, we dont need to create a callback at all

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
  } else if (input == NULL) {
    newState = !curState;
  }
  
  if (curState != newState) {
    setParamInt("state", newState);
    if (newState  ||  input == NULL) onActivated(source, input, output, newState);
  }
}
