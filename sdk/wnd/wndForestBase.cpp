/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "../rmxBase.h"
#include "wndForestBase.h"


whkColor *wndForestBase::hook = NULL;


wndForestBase::wndForestBase(wndForestBase *parentobject, wndForestBase *insert) : wndLabel(parentobject) {
  insertHookWnd(this);
  parent = parentobject;
  cheight = 19;
  resizing = TRUE;

  if (parent) {
    expanded = FALSE;
    root = parent->getRoot();
    depth = parent->getDepth() + 1;
    parent->insertForestObject(this, insert);
    setX(parent->getOffsetX());
  } else {
    expanded = TRUE;
    root = this;
    depth = 0;
  }

  appendWindowStyle(WS_TABSTOP);
  setBackgroundBrush(NULL);
}


wndForestBase::~wndForestBase() {
  resizing = TRUE;
  hideWindow();
  deleteChildren();
  removeForestHook(this);
  if (parent) parent->removeForestObject(this);
}


void wndForestBase::deleteChildren() {
  wndForestBase *object = children.getFirstItem();
  while (object) {
    children.removeItem(object);
    delete object;
    object = children.getFirstItem();
  }
}


void wndForestBase::addForestHook(wndBase *wnd) {
  if (!wnd) return;
  if (!hook) hook = new whkColor();
  hook->insertHookWnd(wnd);
}

void wndForestBase::removeForestHook(wndBase *wnd) {
  if (!wnd) return;
  if (!hook) return;
  hook->removeHookWnd(wnd);
  if (hook->getHookWndCount() == 0) {
    delete hook;
    hook = NULL;
  }
}


void wndForestBase::onPostInit() {
  wndBase::onPostInit();
  resizing = FALSE;
}


void wndForestBase::onSetVisible(BOOL vis) {
  wndBase::onSetVisible(vis);
  
  wndForestBase *parent = getParent();
  if (parent) {
    if (vis) parent->positionChildren(0);
    parent->onChildSized(this);
  }
}


void wndForestBase::insertForestObject(wndForestBase *object, wndForestBase *insert) {
  if (!object) return;
  children.insertItem(object, insert);
  onInsertChildObject(object, insert);
}

void wndForestBase::removeForestObject(wndForestBase *object) {
  children.removeItem(object);
  onDelChildObject(object);
  if (getParent()) {
    getParent()->onChildSized(this);
  }
}


BOOL wndForestBase::moveForestChild(wndForestBase *object, wndForestBase *insert) {
  BOOL ret = children.moveItem(object, insert);
  adjustWindowTops();
  return ret;
}


BOOL wndForestBase::moveForestObject(wndForestBase *insert) {
  wndForestBase *parent = getParent();
  if (!parent) return FALSE;
  return parent->moveForestChild(this, insert);
}



void wndForestBase::setCollapseHeight(int h) {
  if (h == cheight) return;
  cheight = h;
  if (isExpanded()) setHeight(getExpandedHeight());
  else setHeight(getCollapseWindowHeight());
  if (getParent()) getParent()->adjustWindowTops();
  invalidate(NULL, TRUE);
//  if (getParent()) getParent()->onChildSized(this);
}

int wndForestBase::getCollapseHeight() const {
  if (!isVisible()) return 0;
  return cheight;
}

int wndForestBase::getExpandedHeight() const {
  if (!isVisible()) return 0;
  int h = getCollapseWindowHeight();
  wndForestBase *forest = children.getFirstItem();
  while (forest) {
    if (forest->isExpanded()) h += forest->getExpandedHeight();
    else h += forest->getCollapseWindowHeight();
    forest = children.getNextItem(forest);
  }
  return h;
}

int wndForestBase::getExpandedClientHeight() const {
  if (!isVisible()) return 0;
  int h = getCollapseHeight();
  wndForestBase *forest = children.getFirstItem();
  while (forest) {
    if (forest->isVisible()) {
      if (forest->isExpanded()) h += forest->getExpandedHeight();
      else h += forest->getCollapseWindowHeight();
    }
    forest = children.getNextItem(forest);
  }
  return h;
}

int wndForestBase::getObjectHeight() const {
  if (isExpanded()) return getExpandedHeight();
  return getCollapseHeight();
}

int wndForestBase::getCollapseWindowHeight() const {
  if (!isVisible()) return 0;
  RECT xheightrect;
  xheightrect.left = 0;
  xheightrect.top = 0;
  xheightrect.right = 1;
  xheightrect.bottom = getCollapseHeight();
  AdjustWindowRectEx(&xheightrect, getWindowStyle(), NULL, getWindowStyleEx());
  return (xheightrect.bottom - xheightrect.top);
}

void wndForestBase::setExpanded(BOOL expand) {
  if (!parent) return;
  expand = !!expand;
  if (expand == expanded) return;
  expanded = expand;
  onSetExpanded();
  if (getParent()) getParent()->onChildSized(this);
}


int wndForestBase::adjustWindowTops(BOOL recursive) {
  int top = getCollapseHeight();

  wndForestBase *child = children.getFirstItem();
  while (child) {
    if (child->isVisible()) {
      child->setY(top);
      if (child->isExpanded()) top += child->getExpandedHeight();
      else top += child->getCollapseWindowHeight();
    }
    child = children.getNextItem(child);
  }
  return top;
}


void wndForestBase::adjustWindowWidths(BOOL recursive) {
  int width = getClientWidth();
  if (getDepth() > 0) width -= 20;

  HDWP group = BeginDeferWindowPos(children.getItemCount());

  wndForestBase *child = children.getFirstItem();
  while (child) {
//    child->setWidth(width);
    DeferWindowPos(group, child->getWnd(), NULL, 0, 0, width, child->getHeight(), SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOREDRAW);
    child = children.getNextItem(child);
  }

  EndDeferWindowPos(group);

  child = children.getFirstItem();
  while (child) {
    child->updateWindowPosition();
    child->fakeResize();
    if (recursive) child->adjustWindowWidths(TRUE);
    child = children.getNextItem(child);
  }
}


int wndForestBase::positionChildren(int top) {
  int h = getCollapseWindowHeight();
  int offsetx = getOffsetX();
  if (!getParent()) { h = 0; }

  if (!isExpanded()) {
    setMarginBottom(0);
    if (!isVisible()) return 0;
    return h;
  }

  int xwidth = getObjectWidth()-offsetx;
  top += ( getCollapseHeight() - h );

  wndForestBase *object = children.getFirstItem();
  while (object) {
    int newh = object->positionChildren(0);
    object->moveresize(object->getX(), h+top, xwidth, newh);
    h += newh;
    object = children.getNextItem(object);
  }
  setMarginBottom(h - getCollapseWindowHeight());
  
  if (!isVisible()) return 0;
  return h;
}


int wndForestBase::resizeObjects() {
  if (getParent()) if (getRoot()) return getRoot()->resizeObjects();
  if (resizing) return -1;
  resizing = TRUE;

  int h = positionChildren(0);

  resizing = FALSE;
  return h;
}


void wndForestBase::onChildSized(wndForestBase *object) {
  if (resizing) return;
  if (!isVisible()) return;
//  if (!isExpanded()) return;

  int height = getCollapseWindowHeight();
  wndForestBase *o = getFirstForestChild();
  
  while (o) {
//    if (o->getVisible()) height += o->getExpandedHeight();
    if (o->isExpanded()) height += o->getExpandedHeight();
    else height += o->getCollapseWindowHeight();
    o = getNextForestChild(o);
  }
  if (getParent()) {
    if (isExpanded()) setHeight(height);
    else setHeight(getCollapseWindowHeight());
    setMarginBottom(getClientHeight() - getCollapseHeight());
    getParent()->onChildSized(this);
  }
  adjustWindowTops();
}


#endif
