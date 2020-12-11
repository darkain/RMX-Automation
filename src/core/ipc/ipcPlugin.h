/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

#if 0

#ifndef __IPCPLUGIN_H__
#define __IPCPLUGIN_H__


#include <Core/RMX Core/SecureLoader.h>
#include <Core/RMX Core/FunctionCall.h>
#include <Core/RMX DLL LIB/DllEnums.h>


class ipcConfigCallback;


class ipcPlugin : public vSecurePlugin {
  friend class ipcConfigCallback;

  public:
    ipcPlugin(HWND wnd, HPLUGIN plugin, UINT client, BOOL ismaster);
    virtual ~ipcPlugin();

  public:
    virtual UINT sendCommand(BYTE *data, int len) const;
    virtual UINT sendString(const char *str, int len=-1);


    inline HWND getWnd() const { return hwnd; }

    inline void setCallback(BOOL cb) { callback = cb; }

//    void setRemotePlugin(UINT r) { remoteplugin = r; }

    //stuff that is inherited from vPlugin:
    //--------------------------------------

    virtual HPLUGIN getPluginID() const { return remotehandle; } //(HPLUGIN)ipcCommand(1, 400); }

    virtual BOOL external() { return TRUE; }
    virtual BOOL removable() { return FALSE; }

    virtual const char *getPathRootName() { return "IPC"; }
//    virtual const char *getName() const { return name.getName(); }


    //standard win32 sendmessage
    UINT sendMessage(UINT message, UINT param1=NULL, UINT param2=NULL) { return SendMessage(getWnd(), message, param1, param2); }

    //specific stuff for RM-X, wich will go thru a translation process
    UINT sendCommand(UINT command, UINT param1=NULL, UINT param2=NULL);

    static BOOL hasWindowLink(HWND wnd) { return windows.hasItem(wnd); }

  private:
    HWND hwnd;
    Name name;

    HPLUGIN remotehandle;

    BOOL callback;
    BOOL master;

    static rmxList<HWND> windows;
};



#endif//__IPCPLUGIN_H__


#endif //0
