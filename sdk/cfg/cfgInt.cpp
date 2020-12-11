/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cfgInt.h"


cfgInt::cfgInt(const char *initname, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
  : cfgBase(initname, "int", parent, insert, paramlist) {

  setValueInt(0);
  setParamInt("min",   0x80000000)->setSavable(false);
  setParamInt("max",   0x7FFFFFFF)->setSavable(false);
  setParam(   "style", "edit"    )->setSavable(false);
}

cfgInt::~cfgInt() {
}

void cfgInt::onFirstLoad() {
	if (hasParam("default")) setValue(getDefault());
}

void cfgInt::setValue(int value, bool defaultval) {
	if (value == getValue()) return;
	if (value < getMin()) value = getMin();
	if (value > getMax()) value = getMax();
	setValueInt(value);
	if (defaultval) setDefault(value);
}


void cfgInt::setMin(int min) {
	if (min == getMin()) return;
	setParamInt("min", min);
}

void cfgInt::setMax(int max) {
	if (max == getMax()) return;
	setParamInt("max", max);
}

void cfgInt::setMinMax(int min, int max) {
	setMin(min);
	setMax(max);
}

int cfgInt::getMin() const {
	return getParamInt("min");
}

int cfgInt::getMax() const {
	return getParamInt("max");
}

void cfgInt::setDefault(int val) {
	setParamInt("default", val)->setSavable(false);
}

void cfgInt::validateUserData(prmBase *userdata) {
	if (!userdata->isInteger()) userdata->setValueInt( userdata->getValueInt() );
	if (userdata->getValueInt() < getMin()) userdata->setValueInt(getMin());
	if (userdata->getValueInt() > getMax()) userdata->setValueInt(getMax());
}


RMXLONG cfgInt::onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
	if (message == GENERIC_GAIN_FOCUS) onGainFocus(source);
	if (message == GENERIC_LOST_FOCUS) onLostFocus(source);
	return cfgBase::onMessage(source, message, param1, param2);
}

void cfgInt::cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert) {
	cfgBase::cb_cfgMessage(message, item, param, insert);
	if (!param  ||  message != CB_PARAM_UPDATED) return;
	if (param->isNamed("min") || param->isNamed("max")) onSetMinMax();
}


