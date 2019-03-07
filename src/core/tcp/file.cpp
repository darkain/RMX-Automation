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

#include "../loader.h"
#include "../sqlite/sqlite.h"

#include "file.h"




httpFile::httpFile(JNL_HTTPServ *httpserver, rmxDataList *url_list) : server(httpserver), list(url_list) {
	file = NULL;

	const char *ext = "";
	const char *t = list->getFirstItem()->getValue();
	while (*t) {
		if (*t == '.') ext = t + 1;
		t++;
	}


	char path[MAX_PATH] = "";
	VPRINTF(path, sizeof(path), "%s\\rmx_web\\%s", vHPLUGIN::getMaster()->getPath(), list->getFirstItem()->getValue());

	VFOPEN(&file, path, "r+b");


	if (!file) {
		httpserver->set_reply_string("HTTP/1.1 404 NOT FOUND");
	} else {
		httpserver->set_reply_string("HTTP/1.1 200 OK");

		rmxString mime("Content-Type:");
		mime += plg->sql()->getMimeType(ext);
		httpserver->set_reply_header((char*)(const char*)mime);

		//TODO: pass Content-Length and other various HTTP headers
	}
	httpserver->send_reply();
}


httpFile::~httpFile() {
  if (file) fclose(file);
}


int httpFile::GetData(char *buf, int size) {
  if (!file) return 0;
  return fread(buf, 1, size, file);
}
