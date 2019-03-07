/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CONFIGWND_H_
#define _CONFIGWND_H_


#include "../../../sdk/wnd/wndLabel.h"
#include "../../../sdk/whk/whkCommand.h"
#include "../../../sdk/cb/cbRoot.h"
#include "../../core/callbacks/cbPlugin.h"


class vPluginTreeItem;
class wndStatus;
class wndTreeItem;
class vConfigWndTree;


class vConfigWnd : public wndBase/*, public whkCommand*/, public vPluginCallback {
  private:
    vConfigWnd();
    virtual ~vConfigWnd();

  protected:
    virtual void onInit();

//    virtual BOOL onCommand(whkInfo *info);
  
  public:
    void enumList(HPLUGIN plugin, HLIST list, wndTreeItem *parent=NULL);

    wndBase *getPane() { return pane; }
    void setPane(wndBase *wnd) { pane = wnd; }

    void setActiveItem(vPluginTreeItem *treeitem);

    virtual void onResize(int width, int height);

    void setTitle(const char *titletext);

    int getDivider() { return divider; }
    void setDivider(int div);
    int getDividerWidth() { return dividerwidth; }
    void setDividerWidth(int divw);

    
    virtual void onAppendPlugin(vPlugin *plugin);
    
    inline void appendRootCallback(cbRoot *callback) { rootcallbacks.appendItem(callback); }
    inline void removeRootCallback(cbRoot *callback) { rootcallbacks.removeItem(callback); }


    inline static vConfigWnd *getConfigWnd() { return configwnd; }

    void selectItem(vPlugin *plugin, HLIST list);


    static wndBase *config_create(vPlugin *plugin=0, HLIST list=0);
    static void config_destroy();

  protected:
    wndStatus *status;

    vConfigWndTree *treeWnd;
    vPluginTreeItem *item;
    wndBase  *pane;
    wndBase  *rebar;
    wndLabel *title;
    wndBase  *dividerwnd;

//    wndBase *btnSaveSettings;
//    wndBase *btnLoadSettings;

    int divider;
    int dividerwidth;

    int oldw;
    int oldh;

    rmxList<cbRoot*> rootcallbacks;

    static vConfigWnd *configwnd;
};


#endif//_CONFIGWND_H_
