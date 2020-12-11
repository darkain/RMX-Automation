/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


//first off, see if we even need the IPC to begin with
#include "../../../sdk/rmxBase.h"
#ifdef RMX_IPCLOADER

//#includeK/vConfigList/ConfigMenu.h>
#include "../Loader.h"
#include "../LocalPlugin.h"
//#include "ipcMaster.h"
//#include "ipcClient.h"
#include "ipcParser.h"
//#include "ipcConfigCallback.h"
//#include <winsock.h>

//#pragma comment(lib, "Ws2_32.lib")


//---------------------------------------------------------------------------------------------

ipcParser::ipcParser(HWND master, vPlugin *plug) : vSecureParser(plug, NULL) {
  hwnd = master;
  //i need to get the remote and set it
}

ipcParser::~ipcParser() {
}


BOOL ipcParser::valid(HLIST list) {
  if (!list) return FALSE;
  LISTSECURITY sec;
  sec.flags = plugin->getConfigListFlags(list);
  return sec.ipc;
}

BOOL ipcParser::valid(HPARAM param) {
  if (!param) return FALSE;
  LISTSECURITY sec;
  sec.flags = plugin->getListParamFlags(param);
  return sec.ipc;
}

UINT ipcParser::sendCommand(BYTE *data, int len) const {
  COPYDATASTRUCT cds;
  cds.dwData = 425;
  cds.cbData = len;
  cds.lpData = data;
  return (UINT) SendMessage(getWnd(), WM_COPYDATA, (WPARAM)plg->getAppWnd(), (LPARAM)&cds);
}

UINT ipcParser::sendString(const char *str, int len) {
  if (!str) return NULL;
  COPYDATASTRUCT cds;
  cds.dwData = 426;
  cds.lpData = (void*)str;
  if (len == -1) cds.cbData=VSTRLEN(str)+1; else cds.cbData=len;
  return SendMessage(getWnd(), WM_COPYDATA, (WPARAM)plg->getAppWnd(), (LPARAM)&cds);
}


#endif //RMX_IPCLOADER
