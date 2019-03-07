/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../../sdk/xml/xmlwrite.h"
#include "../../sdk/cfg/config.h"
#include "Plugin.h"
#include "callbacks/cbConsole.h"
#include "callbacks/plugCallback.h"


vPlugin::vPlugin(const char *fname, HMODULE hmod) {
  ZeroMemory(&extrmxmsg, sizeof(extrmxmsg));
  extrmxmsg.plugin = (HPLUGIN) this;

  if (fname) {
    rmxString tmpname(fname);
    const char *str = tmpname;
	char *pos = ((char*)str) + tmpname.length();

    while (1) {
      pos--;
      if (pos == str) {
        filename = fname;
        break;
      }

      else if (*pos == '\\') {
        *pos = NULL;
        pos++;
        path     = str;
        filename = pos;
        break;
      }
    }
  }

  hMod  = hmod;
  entry = NULL;
}


vPlugin::~vPlugin() {
  plugCallback *cb = cblist.getFirstItem();
  while (cb) {
#ifdef _DEBUG
    char str[1024];
    VPRINTF(str, sizeof(str), "Undeleted List Callback : %p", cb);
    CONSOLE_MAIN->printWarning("WARNING", str);
#endif

    cb->dstplug = NULL;
    delete cb;
    cblist.removeItem(cb);
    cb = cblist.getFirstItem();
  }

  plg->removePlugin(this);
  if (hMod) { FreeLibrary(hMod); hMod = NULL; }
}


BOOL vPlugin::root_hasList(HLIST list, BOOL recursive) {
  if (!list) return FALSE;
  if (list == list_getRoot()) return TRUE;

  if (recursive) {
    if (list_hasChildCached(list_getRoot(), list)) return TRUE;
  }

  return list_hasChild(list_getRoot(), list, recursive);
}


#ifndef RMX_STANDALONE
BOOL vPlugin::blockMessage(RMXLONG message) const {
  if (plg->isMaster(this)) return FALSE;

  switch (message) {
    case FUNCTION_ACTIVATE:
    case FUNCTION_GET_NAME:
      return TRUE;
  }

  return FALSE;
}
#endif


void vPlugin::insertCallback(HLIST list, HPLUGIN src, void *callback) {
  cblist.appendItem(new plugCallback(this, list, plg->getPlugin(src), callback));
}


void vPlugin::removeCallback(void *callback) {
  plugCallback *cb = getCallback(callback);
  cblist.removeItem(cb);
  delete cb;
}


plugCallback *vPlugin::getCallback(void *callback) {
  plugCallback *cb = cblist.getFirstItem();
  while (cb) {
    if (cb->getCallback() == callback) return cb;
    cb = cblist.getNextItem(cb);
  }
  return NULL;
}


//----------------------------------------------------------------------------------------------------------------


void vPlugin::RmxMsg(vMSG *msg) const {
  (entry)(msg);
}


void vPlugin::RmxMsg(UINT type, UINT msg) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, int p0) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0i  =        p0;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, const void *p1) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1p  = (void*)p1;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, const void *p2) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1p  = (void*)p1;
  extrmxmsg.p2p  = (void*)p2;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, RMXLONG p2, RMXLONG p3, RMXLONG p4) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1p  = (void*)p1;
  extrmxmsg.p2l  =        p2;
  extrmxmsg.p3l  =        p3;
  extrmxmsg.p4l  =        p4;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, int p1, int p2, const void *p3) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1i  =        p1;
  extrmxmsg.p2i  =        p2;
  extrmxmsg.p3p  = (void*)p3;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, int p2, const void *p3) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1p  = (void*)p1;
  extrmxmsg.p2i  =        p2;
  extrmxmsg.p3p  = (void*)p3;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, const void *p2, const void *p3) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1p  = (void*)p1;
  extrmxmsg.p2p  = (void*)p2;
  extrmxmsg.p3p  = (void*)p3;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, RMXLONG p2) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1p  = (void*)p1;
  extrmxmsg.p2l  =        p2;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, double p2) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1p  = (void*)p1;
  extrmxmsg.p2d  =        p2;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, int p2) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1p  = (void*)p1;
  extrmxmsg.p2i  =        p2;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, GUID p23) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1p  = (void*)p1;
  extrmxmsg.p23g =        p23;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, int p1) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1i  =        p1;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, RMXLONG p1) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1l  =        p1;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, double p1) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1d  =        p1;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, RMXLONG p1, int p2) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1l  =        p1;
  extrmxmsg.p2i  =        p2;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, FOURCC p1) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1f  =        p1;
  (entry)((vMSG*)&extrmxmsg);
}


void vPlugin::RmxMsg(UINT type, UINT msg, const void *p0, const void *p1, FOURCC p2) const {
  extrmxmsg.type = type;
  extrmxmsg.msg  = msg;
  extrmxmsg.p0p  = (void*)p0;
  extrmxmsg.p1p  = (void*)p1;
  extrmxmsg.p2f  =        p2;
  (entry)((vMSG*)&extrmxmsg);
}
