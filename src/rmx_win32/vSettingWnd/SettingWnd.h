/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _SETTINGWND_H_
#define _SETTINGWND_H_

#include "../../../sdk/wnd/wndBase.h"
#include "../../../sdk/wnd/wndForest.h"
#include "../../../sdk/wnd/wndForestItems.h"
#include "../../../sdk/wnd/wndDroplist.h"
#include "../../../sdk/whk/whkCommand.h"
#include "../../../sdk/cb/cbCoreDelay.h"
#include "../../core/callbacks/cbFunction.h"


class vSettingWnd;
class wndStatus;
class wndLabel;
class vSettingSaver;


//---------------------------------------------------------------------------------------------------------


class vSettingItem : public cbCoreDelay {
  public:
    vSettingItem(HPLUGIN cb_plugin, HLIST cb_list);
    virtual ~vSettingItem();

  protected:
    virtual void onNameChange(const char *newname);
//    virtual void onInsertChild(vConfigList *child, vConfigList *insert);

//    virtual void onSetVisible(BOOL visible);
//    virtual void onSetEnabled(BOOL enabled);
//    virtual void onSetExpanded(BOOL expanded);
//    virtual void onSetHighlighted(BOOL highlighted);
  public:
    void setForestItem(wndForestItem *fitem);
    wndForestItem *getForstItem() const { return item; }

    virtual const char *getSubOptionName();
    virtual const char *getSubOptionValue() { return NULL; }
    virtual void setSubOptionValue(const char *value) {}

  private:
    wndForestItem *item;
};


//---------------------------------------------------------------------------------------------------------


class vSettingWnd : public wndBase, public cbCore, public whkCommand {
  public:
    vSettingWnd(HPLUGIN plug, HLIST list);
    virtual ~vSettingWnd();

    static vSettingWnd *createWindow(HPLUGIN plug, HLIST list);

    static void destroyAll();

  protected:
    virtual void onInit();
    virtual void onResize(int width, int height);

    virtual BOOL onClose() { return TRUE; }

    virtual BOOL onCommand(whkInfo *info);

//    void enumParam(HPARAM param);

    void applySettings();

  private:
    wndStatus *status;
    wndLabel  *name;
    wndLabel  *path;
    wndForest *forest;

    wndBase *btnApply;
    wndBase *btnOk;
    wndBase *btnCancel;

    rmxList<vSettingSaver*> params;

    static rmxList<vSettingWnd*> wnds;
};


class vSettingSaver {
  protected:
    vSettingSaver() {}
    virtual ~vSettingSaver() {}
  public:
    virtual void onSettingSaver() {}
};


struct functionitem {
  HPLUGIN  paramplugin;
  HLIST    paramlist;

  HPLUGIN  optionplugin;
  HLIST    optionlist;
};


class vSettingFunction : public vDroplistItem {
  public:
    vSettingFunction(const char *name, functionitem *itemdata);
    virtual ~vSettingFunction();

    HPLUGIN getPlugin() { return plugin; }
    HLIST   getList()   { return option; }

  private:
    HPLUGIN plugin;
    HLIST option;
};


class vSettingDroplist : public wndForestItem, public vSettingSaver {
  public:
    vSettingDroplist(wndForestBase *parent, HPLUGIN vplugin, HLIST hlist);
    virtual ~vSettingDroplist();

    void onSelectItem(cbCoreFunction *cb);

    void popup();

  protected:
    virtual void onInit();
    virtual void onResize(int width, int height);
    virtual BOOL onCommand(whkInfo *info);

//    void enumList(HPLUGIN plugin, HLIST list, vMenu  *menu);
    void enumList(cbCoreFunction *cb, vMenu *menu);

    void updateName(cbCoreFunction *cb);

    virtual void onSettingSaver();

    void updateSubs();
    vSettingItem *addSub(HPLUGIN plugin, HLIST list);

  private:
    BOOL loading;

    HPLUGIN plugin;
    HLIST   list;

//    vPlugin *selplugin;
//    HLIST sellist;
    cbCoreFunction *selected;

    wndBase *back;
    wndBase *button;

    whkMouse *hook;

    rmxList<vSettingItem*> subs;
};


//----------------------------------------------------------------------------------------------------


class vDropMenuClick : public whkMouse {
  public:
    vDropMenuClick(vSettingDroplist *dlist) { list = dlist; setReturnDefault(TRUE); }
    virtual ~vDropMenuClick() {}

  protected:
    virtual void onLeftDown(int x, int y) { list->popup(); }
//    virtual void onMouseMove(int x, int y, HWND hwnd) { list->setMouseOver(TRUE); }

  private:
    vSettingDroplist *list;
};


//----------------------------------------------------------------------------------------------------


class vDropMenuItem : public cbCore, public vMenu {
  public:
    vDropMenuItem(cbCoreFunction *cb, vMenu *parent, vSettingDroplist *dlist);
    virtual ~vDropMenuItem() {}

    virtual void onSelect();

  private:
    vSettingDroplist *droplist;
    cbCoreFunction *callback;
};


//----------------------------------------------------------------------------------------------------



/*
class vSettinDropListWnd : public vDroplistWnd {
  public:
    vSettinDropListWnd(vSettingDroplist *parent) : vDroplistWnd(parent) { par = parent; }
    virtual ~vSettinDropListWnd() {}

  protected:
    virtual void onSelectionChange() { par->onSelectionChange(); }

    virtual vDroplistItem *newDroplistItem(const char *name, UINT data=0) {
      return new vSettingFunction(name, (functionitem*)data);
    }

  private:
    vSettingDroplist *par;
};
*/

class vSettingSubDropList : public wndForestItem, public vSettingItem {
  public:
    vSettingSubDropList(HPLUGIN plugin, HLIST list, wndForestBase *parentobject, wndForestBase *insert=NULL);
    virtual ~vSettingSubDropList() {}

  protected:
    virtual void onInit();
    virtual void onResize(int width, int height);
    virtual const char *getSubOptionValue();
    virtual void setSubOptionValue(const char *value);

  private:
    wndDroplist *droplist;
};

class vSettingSubDropListEdit : public wndForestItem, public vSettingItem {
  public:
    vSettingSubDropListEdit(HPLUGIN plugin, HLIST list, wndForestBase *parentobject, wndForestBase *insert=NULL);
    virtual ~vSettingSubDropListEdit() {}

  protected:
    virtual void onInit();
    virtual void onResize(int width, int height);
    virtual const char *getSubOptionValue();
    virtual void setSubOptionValue(const char *value);

  private:
    wndDroplist *droplist;
};

class vSettingSubDropListEditInt : public vSettingSubDropListEdit {
  public:
    vSettingSubDropListEditInt(HPLUGIN plugin, HLIST list, wndForestBase *parentobject, wndForestBase *insert=NULL);
    virtual ~vSettingSubDropListEditInt() {}

  protected:
//    virtual const char *getSubOptionValue();
    //TODO: gotta override this and force to an int
};


class vSettingSubEdit : public vForestEdit, public vSettingItem {
  public:
    vSettingSubEdit(HPLUGIN plugin, HLIST list, wndForestBase *parentobject, wndForestBase *insert=NULL);
    virtual ~vSettingSubEdit() {}

  protected:
    virtual const char *getSubOptionValue();
    virtual void setSubOptionValue(const char *value);
};

class vSettingSubEditInt : public vSettingSubEdit {
  public:
    vSettingSubEditInt(HPLUGIN plugin, HLIST list, wndForestBase *parentobject, wndForestBase *insert=NULL);
    virtual ~vSettingSubEditInt() {}

  protected:
//    virtual const char *getSubOptionValue();
//    virtual void setSubOptionValue(const char *value);
};


#endif//_SETTINGWND_H_