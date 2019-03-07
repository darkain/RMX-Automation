/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "funcBase.h"
#include "funcGroup.h"
#include "../cfg/cfgRoot.h"


//TODO:  record time of event
//if time since last event isnt X, and event is a new source, dont accept
//maybe only for funcAbsolute?


funcBase::funcBase(const char *function_name, const char *script_name, cfgRoot *parent, HPARAMLIST paramlist)
 : cfgBase(function_name, "function", parent, NULL, paramlist) {

	callback = NULL;
	setParam("hint" )->setSavable(false);
	setParam("style")->setSavable(false);
	setParam("script", script_name)->setSavable(false);
}


funcBase::funcBase(const char *function_name, const char *script_name, funcGroup *parent, HPARAMLIST paramlist)
 : cfgBase(function_name, "function", parent, NULL, paramlist) {

	callback = NULL;
	setParam("hint" )->setSavable(false);
	setParam("style")->setSavable(false);
	setParam("script", script_name)->setSavable(false);
}


funcBase::~funcBase() {
  removeFunctionCallback();
}


void funcBase::onActivated(HPLUGIN source, HLIST input, HLIST output) {
  if (callback) {
    if (callback->getRemotePlugin() != source) {
      if (callback->getRemoteConfig() != input) {
        vdelnull(callback);
      }
    }
  }

  if (!callback) {
    callback = new funcCallback(this, source, input);
  }
}


RMXLONG funcBase::onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
  //TODO: test to ensure that HLISTs passed in are indeed valid
  if (message == FUNCTION_ACTIVATE  &&  isEnabled()) onActivated(source, (HLIST)param1, (HLIST)param2);
  if (message == FUNCTION_GET_NAME) getMessageName(source, (HLIST)param1);
  return cfgBase::onMessage(source, message, param1, param2);
}


void funcBase::validateParam(prmBase *param) {
  if (param->isNamed("style")) {
    param->setReadOnly(TRUE);
  }

  cfgBase::validateParam(param);
}

/*
void funcBase::onSetParam(const prmBase *param) {
  if (param->isNamed("hint")) {
    removeFunctionCallback();
  }

  cfgBase::onSetParam(param);
}
*/

void funcBase::cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert) {
	if (message == CB_PARAM_UPDATED  &&  param != NULL)  {
		if (param->isNamed("hint")) removeFunctionCallback();
	}

	cfgBase::cb_cfgMessage(message, item, param, insert);
}



void funcBase::removeFunctionCallback() {
  vdelnull(callback);
}
