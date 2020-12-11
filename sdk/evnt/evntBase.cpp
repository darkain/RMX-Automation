/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "evntBase.h"
#include "../handles/hplugin.h"


evntBase::evntBase(const char *initname, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : cfgBase(initname, "event", parent, insert, paramlist) {

	setParam("display", "Function Assignment")->setSavable(false);
	setParam("plugin");
	setParam("list");
	setParam("hint")->setSavable(false);

	setParam("value")->setSavable(false);
	setParam("lastvalue")->setSavable(false);

	setParam("style")->setSavable(false);


	//this is a temporary hack until the GUI code is updated
	setParam("text", "Assign Function")->setSavable(false);
	setParam("desc", "")->setSavable(false);


	updateDescription();
}


void evntBase::updateDescription() {
	HPLUGIN plugin = vHPLUGIN::getMaster();
	plugin->list_message(plugin->list_getRoot(), EVENT_GET_NAME, (RMXLONG)getListHandle());
}


void evntBase::onClick(HPLUGIN source) {
	source->list_message(source->list_getRoot(), EVENT_OPEN_GUI, (RMXLONG)getListHandle());
}


void evntBase::cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert) {
	cfgBase::cb_cfgMessage(message, item, param, insert);
	if (!param  ||  message != CB_PARAM_UPDATED) return;

	if (param->isNamed("list")) {
		updateDescription();
	}

	else if (isEnabled()  &&  param->isNamed("value")) {
		if (param->getValueNumber() != getParamNumber("lastvalue")) {
			setParamReadOnly("lastvalue", FALSE);
			setParamNumber("lastvalue", param->getValueNumber());
			//TODO: should we re-enable "read only" mode for "lastvalue" ?

			HPLUGIN plugin = vHPLUGIN::getMaster();
			plugin->list_message(plugin->list_getRoot(), EVENT_ACTIVATE, (RMXLONG)getListHandle());
			onActivated();
		}
	}
}


void evntBase::validateParam(prmBase *param) {
	if (param->isNamed("value")) {
		if (!param->isNumber()) {  //force translation to "number"
			param->setValueNumber( param->getValueNumber() );
		}

		//TODO:  -100 is ONLY for relative
		if (param->getValueNumber() < -100.0) {
			param->setValueNumber(-100.0);
		} else if (param->getValueNumber() > 100.0) {
			param->setValueNumber(100.0);
		}
	}

	else if (param->isNamed("style")  ||  param->isNamed("lastvalue")) {
		param->setReadOnly(TRUE);
	}

	cfgBase::validateParam(param);
}


prmBase *evntBase::onNewExternalParam(const char *name, const char *value) {
	//TODO: figure this one out... is 0x01 a reference to external params?
	prmBase *param = setParam(name, value);
	param->_setSecurityEx(0x01);
	return param;
}


RMXLONG evntBase::onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
	if (message == GENERIC_CLICK) {
		if (isEnabled()  &&  source->isMaster()) {
			onClick(source);
		}
	}
	return cfgBase::onMessage(source, message, param1, param2);
}
