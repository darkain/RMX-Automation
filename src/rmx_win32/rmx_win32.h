/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __RMXWIN32_H__
#define __RMXWIN32_H__


#include "../../sdk/wnd/wndBase.h"
#include "../core/Loader.h"


class vMasterPlugin;


class vRmxWin32 : public vPluginLoader, public wndBase {
  protected:
    vRmxWin32();
    virtual ~vRmxWin32();

  protected:
    virtual void onInit();
    virtual void onQuit();
    virtual void onRmxPostLoad();
    virtual void initMasterPlugin(vLocalPlugin *master);
    virtual void initMasterLists(vLocalPlugin *master) {}


#ifdef RMX_STANDALONE
    virtual BOOL onClose() { destroy(); return FALSE; }
#endif


  public:
    virtual void onRemoteRmxWindow(HWND hwnd, BOOL notify=TRUE);

  protected:
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);
    static BOOL CALLBACK EnumChildProc(HWND wnd, LPARAM lparam);

  private:
    char cache[1024];
    vMasterPlugin *rmx_master;
};


extern vRmxWin32 *RmxWin32;


#endif//__RMXWIN32_H__
