/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_TAB_H_
#define _WND_TAB_H_


#include "wndBase.h"
#include "../whk/whkNotify.h"

#include <commctrl.h>


class wndTab;


class wndTabItem {
  friend class wndTab;

  public:
    wndTabItem(const char *name, wndTab *parent, wndTabItem *insert=NULL);
    virtual ~wndTabItem();

  protected:
    virtual void onSelected() {}
    virtual void onDeselected() {}

  public:
    inline wndTab *getParent() const { return par; }

  private:
    wndTab *par;
};


//-----------------------------------------------------------------------------------------------


class wndTab : public wndBase, public whkNotify {
  friend class wndTabItem;

  public:
    wndTab(wndBase *parent=NULL);
    virtual ~wndTab();

  protected:
    virtual void onInsertItem(wndTabItem *item, wndTabItem *insert) { updateSelectedTabItem(); }
    virtual void onRemoveItem(wndTabItem *item) { updateSelectedTabItem(); }
    void updateSelectedTabItem();

  public:
    void clearList();

    wndTabItem *getSelected() const { return selected; }

    virtual BOOL onNotify(whkInfo *info, NMHDR *notify);

  private:
    rmxList<wndTabItem*> items;
    wndTabItem *selected;
};


#endif //_WND_TAB_H_
