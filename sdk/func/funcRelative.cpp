/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "funcRelative.h"


funcRelative::funcRelative(const char *function_name, const char *script_name, cfgRoot *parent)
 : funcBase(function_name, script_name, parent) {

  setParam("style", "relative");
}


funcRelative::funcRelative(const char *function_name, const char *script_name, funcGroup *parent)
 : funcBase(function_name, script_name, parent) {

  setParam("style", "relative");
}


funcRelative::~funcRelative() {
}


void funcRelative::onActivated(HPLUGIN source, HLIST input, HLIST output) {
  funcBase::onActivated(source, input, output);

  double      delta = source->list_getParamNumber(input, "value");
  const char *style = source->list_getParamSafe(  input, "style");

  if (!VSTRCMP(style, "relative")) {
    onActivated(source, input, output, delta);
  } else if (!VSTRCMP(style, "absolute")) {
  } else if (!VSTRCMP(style, "press")) {
  }
}
