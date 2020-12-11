/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"

#include "rmxMasterPlugin.h"

#include "callbacks/cbFunction.h"


rmxMasterPlugin::rmxMasterPlugin(const char *path) : vLocalPlugin("Master", path) {
}


rmxMasterPlugin::~rmxMasterPlugin() {
}


RMXLONG rmxMasterPlugin::onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2) {
	switch (message) {
		case EVENT_ACTIVATE:
			if (isEnabled()) {
				cbCoreFunction *cb = cbCoreFunction::getFunction(source, (HLIST)param1, 2);
				if (cb) cb->activateFunction(source, param1, param2);
			}
		break;


		case EVENT_GET_NAME: {
			cbCoreFunction *cb = cbCoreFunction::getFunction(source, (HLIST)param1);
			if (cb) {
				cb->setEventDescription(source, param1, param2);
			} else {
				cbCoreFunction::setDefaultDescription(source, param1, param2);
			}
		} break;


		case FUNCTION_DEPTH: {
			return static_cast<RMXLONG>(cbCoreFunction::getQueDepth());
		} break;
  }

  return vLocalPlugin::onMessage(source, message, param1, param2);
}
