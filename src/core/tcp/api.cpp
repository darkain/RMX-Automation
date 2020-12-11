/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"
#include "../../../sdk/rmxString.h"
#include "../../../sdk/StringList.h"
#include "../../../sdk/win32/win32.h"

#include "../callbacks/cbFunction.h"

#include "../rmxMasterPlugin.h"

#include "api.h"



httpApi::httpApi(JNL_HTTPServ *httpserver, rmxDataList *url_list) : httpBase(httpserver), list(url_list) {
	int api_count = 1;
	rmxData *api  = list->getItem(api_count);
	cbCoreFunction *api_item = cbCoreFunction::getFunctionByScriptName(api->getValue(), false);

	while (api  &&  api_item) {
		if (api_item->isFunction()) break;
		rmxData *api = list->getItem(++api_count);
		if (api) api_item = api_item->getChildFunctionByScriptName(api->getValue(), false);
	}

	if (!api  ||  !api_item) {
		httpserver->set_reply_string("HTTP/1.1 404 NOT FOUND");
		httpserver->send_reply();
		return;
	}
	
	cfgBase *root    = plg->getMasterPlugin();
	cfgBase *vstack  = new cfgBase("rmx_stack",  "rmx_stack",  root);
	cfgBase *vreturn = new cfgBase("rmx_return", "rmx_return", root);

	vreturn->getParam()->setValue("test");

	//Populate variables passed from HTTP GET to vstack
	const char *t = httpserver->get_request();
	while (*t) t++;
	t++;
	while (*t) {
		while (*t == '&') t++;
		vstack->setParamValue(t, '=');
		t += strlen(t)+1;
	}


	//activate function
	vstack->setParam("style", "press");
	vstack->setParamInt("value", 100);
	api_item->activateFunction(plg->getMasterHandle(), (RMXLONG)vstack, (RMXLONG)vreturn);

	//deactivate function
	vstack->setParamInt("value", 0);
	api_item->activateFunction(plg->getMasterHandle(), (RMXLONG)vstack, (RMXLONG)vreturn);

	rmxString output("{");
	prmBase *param = vreturn->getFirstParam();
	while (param) { //TODO: sanatize this output!
		output += "\"";
		output += param->getName();
		output += "\":\"";
		output += param->getValue();
		output += "\",";
		param = vreturn->getNextParam(param);
	}
	output += "\"rmx_api\":\"2.0\"}";

	setBuffer(output);

	httpserver->set_reply_string("HTTP/1.1 200 OK");
	httpserver->set_reply_header("Content-Type:application/json");
	httpserver->send_reply();
}


httpApi::~httpApi() {
}
