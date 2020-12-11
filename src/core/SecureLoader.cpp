/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

#if 0

#include <rmxBase.h>
#include "SecureLoader.h"
#include "SecureParser.h"
#include "ListCallback.h"
#include <winsock.h>


vSecurePlugin::vSecurePlugin(UINT client) : vPlugin(NULL), vSecureParser(NULL, client) {
}


vSecurePlugin::~vSecurePlugin() {
}



void *vSecurePlugin::registerListCallback(rmxListCallback *callback) {
#ifdef _DEBUG
  char str[1024];
  VPRINTF(str, sizeof(str), "\033[1;35m[IPC LIST CALLBACK] New: CALLBACK=%08X", callback);
  plg->printConsole(str, CONSOLE_MAIN);
#endif
  vPlugin::registerListCallback(callback);
  return (void*)RmxMsg(3, -1, 0, (UINT)callback->getList(), (UINT)callback, (UINT)getClient());
}

void vSecurePlugin::deregisterListCallback(rmxListCallback *callback) {
#ifdef _DEBUG
  char str[1024];
  VPRINTF(str, sizeof(str), "\033[1;35m[IPC LIST CALLBACK] Delete: CALLBACK=%08X", callback);
  plg->printConsole(str, CONSOLE_MAIN);
#endif
//  RmxMsg(3, DELETECALLBACK, callback->getRemoteHandle(), (UINT)callback);
  vPlugin::deregisterListCallback(callback);
}

#endif //0