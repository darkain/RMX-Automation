/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_FOREST_BASE_H_
#define _WND_FOREST_BASE_H_


#include "wndLabel.h"
#include "../whk/whkMouse.h"
#include "../whk/whkColor.h"


class wndForestItem;
class wndForestWnd;

/*
class vForestHook : public vColorWndHook {
  public:
    vForestHook() {}
    virtual ~vForestHook() {}
};
*/

class wndForestBase : public wndLabel, public whkMouse {
  protected:
    wndForestBase(wndForestBase *parentobject=NULL, wndForestBase *insert=NULL);
    virtual ~wndForestBase();

  protected:
    void insertForestObject(wndForestBase *object, wndForestBase *insert);
    void removeForestObject(wndForestBase *object);
    virtual void onInsertChildObject(wndForestBase *object, wndForestBase *insertAfter) {}
    virtual void onDelChildObject(wndForestBase *object) {}

    virtual void onSetVisible(BOOL vis);

    virtual int getObjectWidth() { return getClientWidth(); }

    virtual void onPostInit();

    virtual int getOffsetX() { return 0; }

  public:
    BOOL moveForestObject(wndForestBase *insert);
    BOOL moveForestChild(wndForestBase *child, wndForestBase *insert);

    virtual void onChildSized(wndForestBase *object);

    virtual wndBase *getForestWindow() { return this; }

    int adjustWindowTops(BOOL recursive=FALSE);
    void adjustWindowWidths(BOOL recursive=TRUE);

    int getChildCount() { return children.getItemCount(); }

    void setCollapseHeight(int h);
    int getCollapseHeight() const;
    int getCollapseWindowHeight() const;
    int getExpandedHeight() const;
    int getExpandedClientHeight() const;
    int getObjectHeight() const;

    void setExpanded(BOOL expand);
    BOOL isExpanded() const { return expanded; }
    void toggleExpand() { setExpanded(!isExpanded()); }
    virtual void onSetExpanded() {}

    wndForestBase *getParent() { return parent; }
    wndForestBase *getRoot() { return root; }
    wndForestBase *getFirstForestChild() { return children.getFirstItem(); }
    wndForestBase *getNextForestChild(wndForestBase *c) { return children.getNextItem(c); }

    void deleteChildren();

    int getDepth() const { return depth; }
    BOOL isRoot() const { return (depth == 0); }

    virtual void onLeftDown(int x, int y) { setFocus(); }

    int resizeObjects();

  protected:
    int positionChildren(int top);

  protected:
    void addForestHook(wndBase *wnd);
    void removeForestHook(wndBase *wnd);

  private:
    static whkColor *hook;

  private:
    BOOL resizing;
    int  cheight;
    int  depth;
    BOOL expanded;
    rmxList<wndForestBase*> children;
    wndForestBase *parent;
    wndForestBase *root;
};


#endif //_WND_FOREST_BASE_H_
