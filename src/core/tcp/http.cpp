/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifdef _WIN32
#pragma comment(lib, "WS2_32.lib")
#endif


#include "../../../sdk/rmxBase.h"
#include "../../../sdk/StringList.h"
#include "../../../sdk/win32/win32.h"
#include "http.h"
#include "file.h"
#include "api.h"
#include "apiCore.h"




IPageGenerator *rmx_server::onConnection(JNL_HTTPServ *serv, int port) {
	serv->set_reply_header("Server:rmx_automation/2.0");   //TODO: build this string dynamicly!

	rmxString url = serv->get_request_file();
	rmxString ext = "";

	if (url == "/") url = "/index.html";

	int pos = url.reversefind('.', url.length());
	if (pos > 0) ext = ((const char*)url) + pos + 1;


	rmxDataList list((char*)(const char*)url, '/', true);
	if (list.getFirstItem()->isValued("")) list.removeFirstItem();

	rmxData *path = list.getItem(0);
	if (path  &&  path->isValued("api")) {
		path = list.getItem(1);
		if (path  &&  path->isValued("core")) {
			return new httpApiCore(serv, &list);
		}
		return new httpApi(serv, &list);

	} else if (list.getItemCount() == 1) {
		if (!VSTRCMPI(ext, "jpg")  ||  !VSTRCMPI(ext, "gif")  ||
			!VSTRCMPI(ext, "png")  ||  !VSTRCMPI(ext, "bmp")  ||
			!VSTRCMPI(ext, "txt")  ||  !VSTRCMPI(ext, "htm")  ||
			!VSTRCMPI(ext, "html") ||  !VSTRCMPI(ext, "jpeg") ||
			!VSTRCMPI(ext, "ico")  ||  !VSTRCMPI(ext, "css")  ||
			!VSTRCMPI(ext, "js")   ||  !VSTRCMPI(ext, "xml")  ) {
			return new httpFile(serv, &list);
		}
	}

	serv->set_reply_string("HTTP/1.1 404 NOT FOUND");
	serv->send_reply();
	return NULL;
}
	/*

//	rmxString str;
//	str += "<html><head><title>RMX Automation</title></head><body>";
//	str += serv->get_request_file();
//	str += "<br /><pre>";


	rmxData *path = list.getItem(1);
	if (path  &&  path->isValued("api")) {
//		str += "!!API!!\n";

		int api_count = 2;
		rmxData *api  = list.getItem(api_count);
		cbCoreFunction *api_item = cbCoreFunction::getFunctionByScriptName(api->getValue(), false);

		while (api  &&  api_item) {
			if (api_item->isFunction()) break;
			rmxData *api = list.getItem(++api_count);
			if (api) api_item = api_item->getChildFunctionByScriptName(api->getValue(), false);
		}

		if (api_item) {
//			str += "\n\nActivating: ";
//			str += api_item->getName();
//			str += "\n\n";

			cfgBase *root    = plg->getMasterPlugin();
			cfgBase *vstack  = new cfgBase("stack",  "stack",  root);
			cfgBase *vreturn = new cfgBase("return", "return", root);

			{
				const char *t = serv->get_request();
				while (*t) t++;
				t++;
				while (*t)
				{
					while (*t == '&') t++;
					vstack->insertParam(t, '=');
					t += strlen(t)+1;
				}
			}


			//activate function
			vstack->insertParam("style", "press");
			vstack->insertParamInt("value", 100);
			api_item->activateFunction(plg->getMasterHandle(), (RMXLONG)vstack, (RMXLONG)vreturn);

			//deactivate function
			vstack->setParamInt("value", 0);
			api_item->activateFunction(plg->getMasterHandle(), (RMXLONG)vstack, (RMXLONG)vreturn);
		}
	}



//	str += "\n\n\n----\n\n";

	rmxData *item = list.getFirstItem();
	while (item) {
//		str += item;
//		str += "\n";
		item = list.getNextItem(item);
	}
	
//	str += "\n\n\n----\n\n";


	cbCoreFunction *api_item = cbCoreFunction::getFirstFunction();
	while (api_item) {
//		if (api_item->isFunction()) {
//			str += "Function: ";
//		} else {
//			str += "Group: ";
//		}
//		str += api_item->getScriptName();
//		str += "\n";
		api_item = cbCoreFunction::getNextFunction(api_item);
	}


//	str += "</pre></body></html>";

//	serv->set_reply_string("HTTP/1.1 200 OK");
//	serv->set_reply_header("Content-Type:text/html");
//	serv->send_reply();
//	return new MemPageGenerator(strdup(str.get()));

	//serv->set_reply_string("HTTP/1.1 404 NOT FOUND");
	//serv->send_reply();
	//return 0; // no data
}
*/
