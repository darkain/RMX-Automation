/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CONFIGWND_TREE__H_
#define _CONFIGWND_TREE__H_


#include "../../../sdk/wnd/wndTree.h"
#include "../../../sdk/wnd/wndTreeItem.h"


class vPlugin;


class vConfigWndTree : public wndTree {
  public:
    vConfigWndTree(wndBase *parent);
    virtual ~vConfigWndTree() {}
  
    wndTreeItem *addItem(const char *name, wndTreeItem *parent, HPLUGIN plugin, HLIST list);

  protected:
    virtual wndTreeItem *newTreeItem(const char *name, wndTreeItem *parent, UINT param=0);
};


//-------------------------------------------------------------------------------------


class vPluginTreeItem : public wndTreeItem, public cbCore {
  public:
    vPluginTreeItem(const char *name, wndTreeItem *parent, HPLUGIN plug, HLIST pluglist);
    virtual ~vPluginTreeItem();

  protected:
    virtual void onInit();

    virtual void onSelected();

    virtual void cb_onNameChange(const char *newname);
    virtual void cb_onInsertChild(HLIST child, HLIST insert);

    virtual void onSetExpanded(   BOOL expanded);
    virtual void cb_onSetExpanded(BOOL expanded);

    virtual void cb_onSetVisible(BOOL isvisible);
};


#endif //_CONFIGWND_TREE__H_
