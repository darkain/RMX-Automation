/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

#if 0

#include "../../sdk/rmxBase.h"
#include "ipcPlugin.h"
#include <winsock.h>
#include "../LocalPlugin.h"
#include "../ListCallback.h"


rmxList<const char*> ipcPlugin::stringTable;
rmxList<HWND> ipcPlugin::windows;


ipcPlugin::ipcPlugin(HWND wnd, HPLUGIN plugin, UINT client, BOOL ismaster) : vSecurePlugin(client) {
  callback = TRUE;
  master = ismaster;

  hwnd = wnd;
  remotehandle = plugin;

  windows.appendItem(wnd);

  vSecureParser *parser = this;
  RmxMsg(1, 410, (UINT)parser);  //set remote client info

  char str[1024] = "";
  GetWindowText(hwnd, str, sizeof(str));
  name.setName(str);

  if (master) {
    char str2[1050] = "";
    VPRINTF(str2, sizeof(str2), "\033[33;1mIPC Session: \033[39;1m\"%s\"", str);
    plg->printConsole(str2, CONSOLE_MAIN);
  }

#ifdef _DEBUG
  VPRINTF(str, sizeof(str), "\033[1;35m[IPC] New: HWND=%08X  PLUGIN=%08X  ID=%08X  CLIENT=%08X  NAME=%s", wnd, this, plugin, client, getName());
  plg->printConsole(str, CONSOLE_MAIN);
#endif
}


ipcPlugin::~ipcPlugin() {
#ifdef _DEBUG
  char str[1024] = "";
  VPRINTF(str, sizeof(str), "\033[1;35m[IPC] Delete: PLUGIN=%08X  NAME=%s", this, getName());
  plg->printConsole(str, CONSOLE_MAIN);
#endif

  rmxListCallback *cb = callbacks.getFirstItem();
  while (cb) {
    delete cb;
    callbacks.removeItem(cb);
    cb = callbacks.getFirstItem();
  }

  if (master) {
    char str2[1050] = "";
    VPRINTF(str2, sizeof(str2), "\033[33;1mIPC End: \033[39;1m\"%s\"", getName());
    plg->printConsole(str2, CONSOLE_MAIN);
  }

  plg->removePlugin(this);
  if (callback) SendMessage(getWnd(), WM_USER+303, (UINT)plg->getAppWnd(), 0);
  windows.removeItem(getWnd());
}


UINT ipcPlugin::sendCommand(BYTE *data, int len) const {
  COPYDATASTRUCT cds;
  cds.dwData = 425;
  cds.cbData = len;
  cds.lpData = data;
  return (UINT) SendMessage(getWnd(), WM_COPYDATA, (WPARAM)plg->getAppWnd(), (LPARAM)&cds);
}


UINT ipcPlugin::sendString(const char *str, int len) {
  if (!str) return NULL;
  COPYDATASTRUCT cds;
  cds.dwData = 426;
  cds.lpData = (void*)str;
  if (len == -1) cds.cbData=VSTRLEN(str)+1; else cds.cbData=len;
  return SendMessage(getWnd(), WM_COPYDATA, (WPARAM)plg->getAppWnd(), (LPARAM)&cds);
}



UINT ipcPlugin::sendCommand(UINT command, UINT param1, UINT param2) {
  return sendMessage(0x1000+command, param1, param2);
}


#endif //0
