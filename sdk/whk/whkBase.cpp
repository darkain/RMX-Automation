/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "whkBase.h"


whkBase::whkBase(wndBase *parentwnd) {
  returnDefault = TRUE;
  insertHookWnd(parentwnd);
}


whkBase::~whkBase() {
  wndBase *wnd = parents.getFirstItem();
  while (wnd) {
    wnd->delWndHook(this);
    parents.removeItem(wnd);
    wnd = parents.getFirstItem();
  }
}


void whkBase::insertHookWnd(wndBase *wnd, wndBase *insert) {
  if (!wnd  ||  parents.hasItem(wnd)) return;
  parents.insertItem(wnd, insert);
  wnd->addWndHook(this);
  onInsertWnd(wnd);
}


void whkBase::removeHookWnd(wndBase *wnd) {
  if (!parents.hasItem(wnd)) return;
  parents.removeItem(wnd);
  wnd->delWndHook(this);
}


HWND whkBase::getHookWnd() const {
  wndBase *parent = parents.getFirstItem();
  return (parent) ? (parent->getWnd()) : (NULL);
}


void whkBase::onDeleteWnd(wndBase *wnd) {
  if (!wnd) return;
  wnd->delWndHook(this);
  parents.removeItem(wnd);
}
