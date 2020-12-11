/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/
#if 0

#ifndef __IPCCONFIGCALLBACK_H__
#define __IPCCONFIGCALLBACK_H__


#include <Core/RMX DLL LIB/DllEnums.h>
#include "../ListCallback.h"
#include "ipcPlugin.h"


class ipcConfigCallback : public rmxListCallback {
  public:
    ipcConfigCallback(HLIST locallist, vPlugin *localplugin, UINT remotelist, vSecureParser *remoteplugin)
      : rmxListCallback(locallist, localplugin) {
      remote = remotelist;
      plugin = remoteplugin;
#ifdef _DEBUG
  char str[1024];
  VPRINTF(str, sizeof(str), "\033[1;35m[IPC LIST CALLBACK] New: THIS=%08X  CONFIGLIST=%08X  PLUGIN=%08X  REMOTELIST=%08X  REMOTEPLUGIN=%08X  NAME=%s", this, getList(), getPlugin(), remote, plugin, getListName());
  plg->printConsole(str, CONSOLE_MAIN);
#endif
    }

    virtual ~ipcConfigCallback() {
#ifdef _DEBUG
  char str[1024];
  VPRINTF(str, sizeof(str), "\033[1;35m[IPC LIST CALLBACK] Delete: THIS=%08X  CONFIGLIST=%08X  PLUGIN=%08X  REMOTELIST=%08X  REMOTEPLUGIN=%08X  NAME=%s", this, getList(), getPlugin(), remote, plugin, getListName());
  plg->printConsole(str, CONSOLE_MAIN);
#endif
    }

//    inline UINT sendString(const char *str) { return plugin->sendString(str); }

    virtual void onDelete() { plugin->secureCommand(3, ONDELETE, remote); }
    virtual void onNameChange(const char *newname)  { plugin->secureCommand(3, ONNAMECHANGE, remote, newname); }
    virtual void onSetUserData(const vConfigParam *userdata) { plugin->secureCommand(3, ONSETUSERDATA, remote, userdata); }

    virtual void onInsertChild(vConfigList *child, vConfigList *insert) { plugin->secureCommand(3, ONINSERTCHILD, remote, (UINT)child, (UINT)insert); }
    virtual void onRemoveChild(vConfigList* child) { plugin->secureCommand(3, ONREMOVECHILD, remote, (UINT)child); }
    virtual void onMoveChild(vConfigList *child, vConfigList *insert) { plugin->secureCommand(3, ONMOVECHILD, remote, (UINT)child, (UINT)insert); }

    virtual void onInsertParam(vConfigParam *param, vConfigParam *insert)    { plugin->secureCommand(3, ONINSERTPARAM, remote, (UINT)param, (UINT)insert); }
    virtual void onRemoveParam(vConfigParam *param) { plugin->secureCommand(3, ONREMOVEPARAM, remote, (UINT)param);   }
    virtual void onSetParam(vConfigParam *param, vConfigParam *oldval) { plugin->secureCommand(3, ONSETPARAM, remote, (UINT)param, (UINT)oldval); }

    virtual void onSetVisible(BOOL visible)         { plugin->secureCommand(3, ONSETVISIBLE,     remote, visible);     }
    virtual void onSetExpanded(BOOL expanded)       { plugin->secureCommand(3, ONSETEXPANDED,    remote, expanded);    }
    virtual void onSetHighlighted(BOOL highlighted) { plugin->secureCommand(3, ONSETHIGHLIGHTED, remote, highlighted); }
    virtual void onSetEnabled(BOOL enabled)         { plugin->secureCommand(3, ONSETENABLED,     remote, enabled);     }
    virtual void onSetSavable(BOOL savable)         { plugin->secureCommand(3, ONSETPROFILABLE,  remote, savable);     }

  private:
    UINT remote;
    vSecureParser *plugin;
};

#endif //__IPCCONFIGCALLBACK_H__

#endif 0

