/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_TREE_ITEM_H_
#define _WND_TREE_ITEM_H_


#include "wndTree.h"


class wndTreeItem : public rmxString {
  friend class wndTree;

  public:
    wndTreeItem(const char *name, wndTreeItem *parent, BOOL isvisible=TRUE);
    virtual ~wndTreeItem();

    virtual void onInit() {}

    virtual void onDeselected();
    virtual void onSelected();

    void select();

    void addTreeChild(wndTreeItem *item);
    void delTreeChild(wndTreeItem *item);
    wndTreeItem *getFirstItem() { return items.getFirstItem(); }
    wndTreeItem *getNextItem(wndTreeItem *i) { return items.getNextItem(i); }
    wndTreeItem *getPrevItem(wndTreeItem *i) { return items.getPrevItem(i); }
    wndTreeItem *getLastItem() { return items.getLastItem(); }
    UINT getItemCount() { return items.getItemCount(); }

    wndTreeItem *getTreeParent() { return treeParent; }
    wndTreeItem *getTreeRoot() { return treeRoot; }

    void setWnd(HWND hWnd) { hTree = hWnd; }
    HWND getWnd() { return hTree; }
    void setWnd(wndTree *w);
    wndTree *getwndBase();

    void setTreeItem(HTREEITEM item) { hItem = item; }
    HTREEITEM getTreeItem() { return hItem; }

    void setExpanded(BOOL isexpand);
    BOOL isExpanded();
    virtual void onSetExpanded(BOOL expanded) {}

    void setVisible(BOOL isvisible);
    BOOL isVisible();
    virtual void onSetVisible(BOOL isvisible) {}

    //from class Name
    virtual void onNameChange(const char *newname);

  protected:
    rmxList<wndTreeItem*> items;
    wndTreeItem *treeParent;
    wndTreeItem *treeRoot;
    HTREEITEM hItem;
    HWND hTree;
    wndTree *wnd;

    BOOL expand;
    BOOL visible;
};


#endif //_WND_TREE_ITEM_H_
