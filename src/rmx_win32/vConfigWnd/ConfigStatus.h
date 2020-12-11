/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CONFIGWND_STATUS_H_
#define _CONFIGWND_STATUS_H_



#include "../../../sdk/wnd/wndStatus.h"
#include "../../../sdk/cb/cbRoot.h"
#include "../../core/callbacks/cbPlugin.h"



class cbRootConfigStatus;
class cbBaseConfigStatusBar;
class cbBaseConfigStatusButton;



class wndConfigStatus : public vButtonStatusWnd, public vPluginCallback {
  friend class cbRootConfigStatus;
  friend class cbBaseConfigStatusButton;

  public:
    wndConfigStatus(wndBase *parent);
    virtual ~wndConfigStatus();

  public:
    cbBaseConfigStatusButton *getStatusButton(int id);

  protected:
    virtual void onAppendPlugin(vPlugin *plugin);
    virtual VUINT subWndProc(UINT msg, VUINT param1, VUINT param2);

  private:
    rmxList<cbRootConfigStatus*>       roots;
    rmxList<cbBaseConfigStatusButton*> items;
};



class cbRootConfigStatus : public cbRoot {
  friend class cbBaseConfigStatusBar;
  
  public:
    cbRootConfigStatus(HPLUGIN plugin, wndConfigStatus *par);
    virtual ~cbRootConfigStatus();

  public:
    inline wndConfigStatus *getParent() { return parent; }

  protected:
    virtual void cb_onInsertChild(HLIST child, HLIST insert);

  private:
    wndConfigStatus *parent;
    rmxList<cbBaseConfigStatusBar*> items;
};


class cbBaseConfigStatusBar : public cbCore {
  friend class cbBaseConfigStatusButton;
  
  public:  
    cbBaseConfigStatusBar(HPLUGIN plugin, HLIST item, cbRootConfigStatus *par, wndConfigStatus *grandpar);
    virtual ~cbBaseConfigStatusBar();

  public:
    inline cbRootConfigStatus *getParent() { return parent; }

  protected:
    virtual void cb_onInsertChild(HLIST child, HLIST insert);

  private:
    wndConfigStatus    *grandparent;
    cbRootConfigStatus *parent;
    rmxList<cbBaseConfigStatusButton*> items;
};


class cbBaseConfigStatusButton : public cbCore {
  public:  
    cbBaseConfigStatusButton(HPLUGIN plugin, HLIST item, cbBaseConfigStatusBar *par, wndConfigStatus *grandpar);
    virtual ~cbBaseConfigStatusButton();

  public:
    inline int getId() const { return id; }

  protected:
    //virtual void cb_onNameChange(const char *newname);
	virtual void cb_cfgMessage(cbMessage message, HPARAM param, HPARAM insert=NULL);

  private:
    wndConfigStatus       *grandparent;
    cbBaseConfigStatusBar *parent;
    int id;
    static int btns;
};



#endif //_CONFIGWND_STATUS_H_
