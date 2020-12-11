/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CONFIGPANEWND_H_
#define _CONFIGPANEWND_H_


#include "../../../sdk/wnd/wndForest.h"
#include "../../../sdk/cb/cbCore.h"


class vConfigWnd;
class vConfigPaneItem;


class vConfigPaneWnd : public wndForest, public cbCore {
  public:
    vConfigPaneWnd(vConfigWnd *parent, HPLUGIN plug, HLIST pluglist);
    virtual ~vConfigPaneWnd();

    virtual void onInit();

//    virtual void cb_onSetParam(HPARAM param);
//    virtual void cb_onInsertChild(HLIST child, HLIST insert);
//    virtual void cb_onMoveChild(HLIST child, HLIST insert);
	virtual void cb_cfgMessage(cbMessage message, HLIST  child, HLIST  insert=NULL);
	virtual void cb_cfgMessage(cbMessage message, HPARAM param, HPARAM insert=NULL);

    void removeConfigChild(wndBase *child);

    void enumChildren(HLIST list, wndForestBase *object, wndForestBase *insert=NULL);

    void setTitle(const char *text);

    wndForestItem *getObjectFromList(const HLIST list) const;
    inline wndForestItem *getObjectFromList(const cfgBase *list) const { return getObjectFromList((const HLIST)list); }

    void addPaneItem(vConfigPaneItem *item) { items.appendItem(item); }
    void delPaneItem(vConfigPaneItem *item) { items.removeItem(item); }

  protected:
    vConfigWnd *config;
    rmxList<vConfigPaneItem*> items;
};


#endif //_CONFIGPANEWND_H_
