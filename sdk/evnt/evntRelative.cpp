/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "evntRelative.h"


evntRelative::evntRelative(const char *initname, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : evntBase(initname, parent, insert, paramlist) {

	setParam("style", "relative");
	setParam("min")->setSavable(false);
	setParam("max")->setSavable(false);
	setMin(-100.0);
	setMax( 100.0);
}


evntRelative::~evntRelative() {
}


void evntRelative::setMin(double min) {
  setParamNumber("min", min);
}


void evntRelative::setMax(double max) {
  setParamNumber("max", max);
}


void evntRelative::setMinMax(double min, double max) {
  setMin(min);
  setMax(max);
}


void evntRelative::setRelativeValue(double value) {
  double range = getMax() - getMin();
  value -= getMin();
  double delta = (value / range);
  delta -= 0.5;
  if (delta < -1.0) delta = -1.0;
  if (delta >  1.0) delta =  1.0;
  setValue(delta * 200);
}

/*
void evntRelative::onSetParam(const prmBase *param) {
  evntBase::onSetParam(param);
*/

void evntRelative::cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert) {
  evntBase::cb_cfgMessage(message, item, param, insert);
  if (!param  ||  message != CB_PARAM_UPDATED) return;

  if (param->isNamed("value")) {
    if (param->getValueNumber() > 50  ||  param->getValueNumber() < -50) {
      setHighlighted(TRUE);
    } else {
      setHighlighted(FALSE);
    }
  }
}
