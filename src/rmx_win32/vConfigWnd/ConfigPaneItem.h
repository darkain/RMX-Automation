/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CONFIGPANEITEM_H_
#define _CONFIGPANEITEM_H_


#include "../../../sdk/cb/cbCoreDelay.h"


class vConfigPaneWnd;
class wndForestItem;


class vConfigPaneItem : public cbCoreDelay {
  public:
    vConfigPaneItem(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list);
    virtual ~vConfigPaneItem();

  protected:
    virtual void onInit();

		virtual void cb_cfgMessage(cbMessage message, const char *text);

		virtual void cb_cfgMessage(cbMessage message, RMXLONG data);

		virtual void cb_cfgMessage(cbMessage message, HLIST child, HLIST insert=NULL);

		virtual void cb_cfgMessage(cbMessage message, HPARAM param, HPARAM insert=NULL);

/*
    virtual void cb_onNameChange(const char *newname);
    virtual void cb_onInsertChild(HLIST child, HLIST insert);
    virtual void cb_onMoveChild(HLIST child, HLIST insert);

    virtual void cb_onSetVisible(BOOL visible);
    virtual void cb_onSetEnabled(BOOL enabled);
    virtual void cb_onSetExpanded(BOOL expanded);
    virtual void cb_onSetHighlighted(BOOL highlighted);
*/
  public:
    void setForestItem(wndForestItem *fitem);
    wndForestItem *getForstItem() const { return item; }

  private:
    vConfigPaneWnd *pane;
    wndForestItem  *item;
};


#endif //_CONFIGPANEITEM_H_
