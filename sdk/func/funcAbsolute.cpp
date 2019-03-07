/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "funcAbsolute.h"


funcAbsolute::funcAbsolute(const char *function_name, const char *script_name, cfgRoot *parent)
 : funcBase(function_name, script_name, parent) {

	setParam("style", "absolute");
	setParamInt("min",    0)->setSavable(false);
	setParamInt("max",  100)->setSavable(false);
	setParamInt("delta", 10)->setSavable(false);
	setParamInt("value",  0)->setSavable(false);
}


funcAbsolute::funcAbsolute(const char *function_name, const char *script_name, funcGroup *parent)
 : funcBase(function_name, script_name, parent) {

	setParam("style", "absolute");
	setParamInt("min",    0)->setSavable(false);
	setParamInt("max",  100)->setSavable(false);
	setParamInt("delta", 10)->setSavable(false);
	setParamInt("value",  0)->setSavable(false);
}


funcAbsolute::~funcAbsolute() {
}


void funcAbsolute::setMinMax(double min, double max) {
  setMin(min);
  setMax(max);
}


void funcAbsolute::onActivated(HPLUGIN source, HLIST input, HLIST output) {
  funcBase::onActivated(source, input, output);

  double      value = source->list_getParamNumber(input, "value");
  const char *style = source->list_getParamSafe(  input, "style");

  if (!VSTRCMP(style, "absolute")) {
    double range  = getMax() - getMin();
    double newval = (value / 100) * range;
    newval += getMin();
    setValue(newval);
    onActivated(source, input, output, newval);

  } else if (!VSTRCMP(style, "relative")) {
    if (value > 10  ||  value < -10) {  //TODO: add "deadzone"
      double range  = getMax() - getMin();
      double newval = (getValue()) + ((value / 100) * getDelta());
      if (newval < getMin()) newval = getMin();
      if (newval > getMax()) newval = getMax();
      setValue(newval);
      onActivated(source, input, output, newval);
    }

  } else if (!VSTRCMP(style, "press")) {
  }
}
