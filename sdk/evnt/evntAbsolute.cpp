/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "evntAbsolute.h"


evntAbsolute::evntAbsolute(const char *initname, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : evntBase(initname, parent, insert, paramlist) {

   setParam("style", "absolute");
   setParam("min")->setSavable(false);
   setParam("max")->setSavable(false);
   setMin(  0.0);
   setMax(100.0);
}


evntAbsolute::~evntAbsolute() {
}


void evntAbsolute::setMin(double min) {
	setParamNumber("min", min);
}


void evntAbsolute::setMax(double max) {
	setParamNumber("max", max);
}


void evntAbsolute::setMinMax(double min, double max) {
	setMin(min);
	setMax(max);
}


void evntAbsolute::setAbsoluteValue(double value) {
	double range = getMax() - getMin();
	value -= getMin();
	double delta = value / range;
	if (delta < 0.0) delta = 0.0;
	if (delta > 1.0) delta = 1.0;
	setValue(delta * 100);
}


void evntAbsolute::cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert) {
	evntBase::cb_cfgMessage(message, item, param, insert);
	if (!param  ||  message != CB_PARAM_UPDATED) return;

	if (param->isNamed("value")) {  //TODO:  change highlighted to being based on when event occures, user timer to timeout
		if (param->getValueNumber() > 75  ||  param->getValueNumber() < 25) {
			setHighlighted(TRUE);
		} else {
			setHighlighted(FALSE);
		}
	}
}
