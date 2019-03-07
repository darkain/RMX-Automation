/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_TREE_H_
#define _WND_TREE_H_


#include "wndBase.h"
#include "../whk/whkNotify.h"
#include <commctrl.h>


class wndTreeItem;
class wndTreeItemList;


class wndTree : public wndBase, public whkNotify {
  friend class wndTreeItem;

  public:
    wndTree(wndBase *parent=NULL);
    virtual ~wndTree();

  public:
    void clearList();

    void addTreeChild(wndTreeItem *item);
    void delTreeChild(wndTreeItem *item);

    inline wndTreeItem *getSelected() const { return selected; }
    inline wndTreeItem *getFirstItem() const { return items.getFirstItem(); }
    inline wndTreeItem *getNextItem(wndTreeItem *i) const { return items.getNextItem(i); }
    inline wndTreeItem *getPrevItem(wndTreeItem *i) const { return items.getPrevItem(i); }
    inline wndTreeItem *getLastItem() const { return items.getLastItem(); }

    inline UINT getItemCount() const { return items.getItemCount(); }
    inline BOOL hasItem(wndTreeItem *i) const { return items.hasItem(i); }

    wndTreeItem *addTreeItem(const char *name, UINT notifyId=0, wndTreeItem *parent=NULL, UINT param=0);
    void addTreeItem(wndTreeItem *treeItem);

  protected:
    virtual wndTreeItem *newTreeItem(const char *name, wndTreeItem *parent, UINT param=0);
    virtual BOOL onNotify(whkInfo *info, NMHDR *notify);

  protected:
    rmxList<wndTreeItem*> items;
    wndTreeItem *selected;
};


#endif //_WND_TREE_H_
