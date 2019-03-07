/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __RMXWIN32_MASTERPLUGIN_H__
#define __RMXWIN32_MASTERPLUGIN_H__


#include "../../../sdk/rmxLock.h"
#include "../../../sdk/cfg/config.h"
#include "../../core/LocalPlugin.h"


class vMasterTray;


class vMasterPlugin : public vLocalPlugin, public rmxLock {
  public:
    vMasterPlugin(const char *path);
    virtual ~vMasterPlugin() {}


  protected:
    virtual void onInitPlugin();
    virtual void onFirstLoad();
//    virtual void onLoadSettings();

    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2);

  private:
    vMasterTray *trayIcon;
    cfgTree *tree;
};


#endif //__RMXWIN32_MASTERPLUGIN_H__
