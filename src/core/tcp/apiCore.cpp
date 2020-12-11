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

#include "apiCore.h"



httpApiCore::httpApiCore(JNL_HTTPServ *httpserver, rmxDataList *url_list) : httpBase(httpserver), list(url_list) {
	rmxData *api  = list->getItem(1);
	rmxData *tree = list->getItem(2);

	//TODO: test which API call and then have a class instance per function call

	rmxString output(16*1024, "{");

	if (tree  &&  tree->getValueLong() > 0) {
		vPlugin *plugin = plg->getFirstPlugin();
		while (plugin) {
			HLIST hlist = plugin->list_getChildById( plugin->list_getRoot(), tree->getValueLong(), true );
			if (hlist) {
				dumpList(plugin, hlist, output);
				break;
			}
			plugin = plg->getNextPlugin(plugin);
		}

	} else {
		vPlugin *plugin = plg->getFirstPlugin();
		while (plugin) {
			dumpList( plugin, plugin->list_getRoot(), output );
			plugin = plg->getNextPlugin(plugin);
		}
	}


	output += "\"rmx_api\":\"2.0\",\"api\":\"";
	output += api;
	output += "\"}";
	setBuffer(output);

	httpserver->set_reply_string("HTTP/1.1 200 OK");
	httpserver->set_reply_header("Content-Type:application/json");
	httpserver->send_reply();
}


httpApiCore::~httpApiCore() {
}


void httpApiCore::dumpList(vPlugin *plugin, HLIST list, rmxString &output, bool last) {
	output += "\"list_";
	output += plugin->list_getId(list);
	output += "\":{\"type\":\"";
	output += plugin->list_getType(list);
	output += "\",\"id\":";
	output += plugin->list_getId(list);

	HPARAM param = plugin->list_getParamFirst(list);
	while (param) {
		HPARAM next = plugin->list_getParamNext(list, param);
		output += ",\"";
		output += plugin->param_getName(param);
		output += "\":\"";
		output += plugin->param_getValue(param);
		output += "\"";
		param = next;
	}

	HLIST child = plugin->list_getFirstChild(list);
	if (child) output += ",";
	while (child) {
		HLIST next = plugin->list_getNextChild(list, child);
		dumpList( plugin, child, output, (next==NULL) );
		child = next;
	}

	if (last) {
		output += "}";
	} else {
		output += "},";
	}
}
