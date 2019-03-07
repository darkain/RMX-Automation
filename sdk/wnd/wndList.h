/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_LIST_H_
#define _WND_LIST_H_


#include "wndBase.h"
#include "../whk/whkCommand.h"


class wndList;


class wndListItem {
  friend class wndList;

  public:
    wndListItem(const char *name, wndList *parent, wndListItem *insert=NULL);
    virtual ~wndListItem();

  protected:
    virtual void onSelected() {}
    virtual void onDeselected() {}
    virtual void onDoubleClick() {}

  public:
    inline wndList *getParent() const { return par; }

  private:
    wndList *par;
};


//-----------------------------------------------------------------------------------------------


class wndList : public wndBase, public whkCommand {
  friend class wndListItem;

  public:
    wndList(wndBase *parent);
    virtual ~wndList();

  public:
    void clearList();

    wndListItem *getSelected() const { return selected; }

    virtual BOOL onCommand(whkInfo *info);

  private:
    rmxList<wndListItem*> items;
    wndListItem *selected;
};


#endif //_WND_LIST_H_
