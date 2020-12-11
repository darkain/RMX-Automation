/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/
#if 0

#ifndef __IPROOTLISTCALLBACK_H__
#define __IPROOTLISTCALLBACK_H__


#include <Core/RMX DLL LIB/DllEnums.h>
#include "../RootListCallback.h"
#include "ipcPlugin.h"


class ipcRootListCallback : public vRootListCallback {
  public:
    ipcRootListCallback(vPlugin *localplugin, UINT remotelist, vSecureParser *remoteplugin) : vRootListCallback(localplugin) {
      remote = remotelist;
      plugin = remoteplugin;
#ifdef _DEBUG
  char str[1024];
  VPRINTF(str, sizeof(str), "\033[1;35m[IPC ROOT CALLBACK] New: PLUGIN=%08X  REMOTELIST=%08X  REMOTEPLUGIN=%08X  NAME=%s", getPlugin(), remote, plugin, getPlugin()->getName());
  plg->printConsole(str, CONSOLE_MAIN);
#endif
    }

    virtual ~ipcRootListCallback() {
#ifdef _DEBUG
  char str[1024];
  VPRINTF(str, sizeof(str), "\033[1;35m[IPC ROOT CALLBACK] Delete: PLUGIN=%08X  REMOTELIST=%08X  REMOTEPLUGIN=%08X  NAME=%s", getPlugin(), remote, plugin, getPlugin()->getName());
  plg->printConsole(str, CONSOLE_MAIN);
#endif
    }

//    inline UINT sendString(const char *str) { return plugin->sendString(str); }
/*
    virtual void onInsertRootList(HLIST list, HLIST insert) {
#ifdef _DEBUG
  char str[1024];
  VPRINTF(str, sizeof(str), "\033[1;35m[IPC ROOT CALLBACK] Insert: LIST=%08X  INSERT=%08X  TYPE=%s  NAME=%s", list, insert, getPlugin()->getConfigListType(list), getPlugin()->getConfigListName(list));
  plg->printConsole(str, CONSOLE_MAIN);
#endif
      plugin->secureCommand(5, ONNEWROOTLIST, remote, (UINT)list, (UINT)insert);
    }
*/
/*
    virtual void onDelete() { plugin->secureCommand(3, ONDELETE, remote); }
    virtual void onNameChange(const char *newname)  { plugin->secureCommand(3, ONNAMECHANGE, remote, (UINT)sendString(newname)); }
    virtual void onSetUserData(const char *newdata) { plugin->secureCommand(3, ONSETUSERDATA, remote, (UINT)sendString(newdata)); }

    virtual void onInsertChild(vConfigList *child, vConfigList *insert) { plugin->secureCommand(3, ONINSERTCHILD, remote, (UINT)child, (UINT)insert); }
    virtual void onRemoveChild(vConfigList* child) { plugin->secureCommand(3, ONREMOVECHILD, remote, (UINT)child); }
    virtual void onMoveChild(vConfigList *child, vConfigList *insert) { plugin->secureCommand(3, ONMOVECHILD, remote, (UINT)child, (UINT)insert); }

    virtual void onInsertParam(vConfigParam *param, vConfigParam *insert)    { plugin->secureCommand(3, ONINSERTPARAM, remote, (UINT)param, (UINT)insert); }
    virtual void onRemoveParam(vConfigParam *param) { plugin->secureCommand(3, ONREMOVEPARAM, remote, (UINT)param);   }
    virtual void onSetParam(vConfigParam *param, vConfigParam *oldval) { plugin->secureCommand(3, ONSETPARAM, remote, (UINT)param, (UINT)oldval); }

    virtual void onSetVisible(BOOL hidden)         { plugin->secureCommand(3, ONSETVisible,     remote, Visible);     }
    virtual void onSetExpanded(BOOL expanded)     { plugin->secureCommand(3, ONSETEXPANDED,   remote, expanded);   }
    virtual void onSetHilighted(BOOL hilited)     { plugin->secureCommand(3, ONSETHILIGHTED,  remote, hilited);    }
    virtual void onSetEnabled(BOOL enable)        { plugin->secureCommand(3, ONSETENABLED,    remote, enable);     }
    virtual void onSetProfilable(BOOL profilable) { plugin->secureCommand(3, ONSETPROFILABLE, remote, profilable); }
    virtual void onSetSavable(BOOL savable)       { plugin->secureCommand(3, ONSETPROFILABLE, remote, savable);    }
*/
  private:
    UINT remote;
    vSecureParser *plugin;
};


#endif //__IPROOTLISTCALLBACK_H__


#endif //0
