/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __RMXWIN32_MASTERTRAY_H__
#define __RMXWIN32_MASTERTRAY_H__


#include "../../../sdk/cfg/cfgBool.h"
#include "../../../sdk/cb/cbCore.h"
#include "../../../sdk/Win32/TrayIcon.h"
#include "../../../sdk/Win32/Menu.h"


class vPlugin;


class vMasterTray : public cfgBool, /*public cbBase,*/ public vTrayIcon {
  public:
    vMasterTray(cfgBase *parent, cfgBase *watch);
    virtual ~vMasterTray();

  public:
    virtual void onFirstLoad() { setValue(TRUE); }

    void popMenu();

  protected:
    //virtual void cb_onSetEnabled(BOOL enabled);
    virtual void cb_cfgMessage(cbMessage message, cfgBase *item, RMXLONG data);

	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert=NULL);


//    virtual void onSetUserData(const prmBase *userdata);
    virtual void onRightClick(int x, int y);
    virtual void onLeftDoubleClick(int x, int y);
    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2);

    void recurse(vMenu *menu, HPLUGIN plugin, HLIST list);

    virtual void onNameChange(const char *newname) {
      cfgBool::onNameChange(newname);
      vTrayIcon::onNameChange(newname);  
    }

  private:
    HICON icon[2];
};


//-------------------------------------------------------------------------------


class vMasterTrayItem : public vMenu, public cbCore {
  public:
    vMasterTrayItem(const char *name, vMenu *parent, HPLUGIN plugin, HLIST list);
    virtual ~vMasterTrayItem();
  
  public:
    virtual void onClick();
	virtual void cb_cfgMessage(cbMessage message, HPARAM param, HPARAM insert=NULL);
//    virtual void cb_onSetParam(HPARAM param);
};


#endif //__RMXWIN32_MASTERTRAY_H__
