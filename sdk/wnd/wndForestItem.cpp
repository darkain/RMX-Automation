/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "../rmxBase.h"
#include "wndForestItem.h"
#include "wndSubclass.h"


wndForestItem::wndForestItem(wndForestBase *parentobject, wndForestBase *insert) : wndForestBase(parentobject, insert) {
  setvParent(parentobject->getForestWindow());
  appendWindowStyleEx(WS_EX_CLIENTEDGE);

  whkCommand::insertHookWnd(this);
  addForestHook(this);

  btn = NULL;
  highlighted = FALSE;
  mouseOver = FALSE;
  itemenable = TRUE;

  setMargins(20, 0, 0, 0);
  setTextVAlign(TEXTVALIGN_MIDDLE);

  if (getDepth() & 0x1) {
    normalbrush = GetSysColorBrush(COLOR_BTNFACE);
    normalcolor = GetSysColor(COLOR_BTNTEXT);
  } else {
    normalbrush = GetSysColorBrush(COLOR_INFOBK);
    normalcolor = GetSysColor(COLOR_INFOTEXT);
  }

  hoverbrush = GetSysColorBrush(COLOR_HIGHLIGHT);
  hovercolor = GetSysColor(COLOR_HIGHLIGHTTEXT);
  updateForestColors();
}


wndForestItem::~wndForestItem() {
  btn = NULL;
}


void wndForestItem::setNormalBrush(HBRUSH brush) {
  normalbrush = brush;
  updateForestColors();
}

void wndForestItem::setHoverBrush(HBRUSH brush) {
  hoverbrush = brush;
  updateForestColors();
}

void wndForestItem::setNormalColor(COLORREF clr) {
  normalcolor = clr;
  updateForestColors();
}

void wndForestItem::setHoverColor(COLORREF clr) {
  hovercolor = clr;
  updateForestColors();
}


void wndForestItem::updateForestColors() {
  if (isInited()) {
    if (isMouseOver() || isHighlighted()) {
      setBackgroundBrush(hoverbrush);
      setTextColor(hovercolor);
      invalidateForestItem();
      return;
    }
  }
  setBackgroundBrush(normalbrush);
  setTextColor(normalcolor);
  invalidateForestItem();
}


void wndForestItem::onInit() {
  wndForestBase::onInit();

  btn = new wndSubclass("Button", this);
  btn->setBackgroundBrush((HBRUSH)GetStockObject(HOLLOW_BRUSH));
  btn->appendWindowStyle(BS_FLAT);
  
  if (isExpanded()) {
    *btn = "-";
  } else {
    *btn = "+";
  }

  btn->setId(300);
  btn->init();
  btn->moveresize(0, 0, 18, 18);
  whkMouse::insertHookWnd(btn);

  updateForestColors();
}


void wndForestItem::onPostInit() {
  wndForestBase::onPostInit();
  onSetItemEnabled(itemenable);
}


void wndForestItem::onInsertChildObject(wndForestBase *object, wndForestBase *insertAfter) {
  wndForestBase::onInsertChildObject(object, insertAfter);
  if (btn) btn->showWindow();
}

void wndForestItem::onDelChildObject(wndForestBase *object) {
  if (btn) if (getChildCount() == 0) btn->hideWindow();
}


void wndForestItem::invalidateForestItem() {
  if (!isExpanded()) {
    invalidate(NULL, TRUE);
    return;
  }

  RECT r;
  int h;
  getClientRect(&r);
  h = r.bottom;

  r.bottom = getCollapseHeight();
  invalidate(&r, TRUE);
  r.top = r.bottom;
  r.bottom = h;
  r.right = r.left + 20;  //this should be changed for variable widths
  invalidate(&r, TRUE);
}


void wndForestItem::setItemEnabled(BOOL en) {
  en = !!en;
  if (en == itemenable) return;
  itemenable = en;
  onSetItemEnabled(itemenable);
}


void wndForestItem::onSetMouseOver(BOOL mouseover) {
  wndForestBase::onSetMouseOver(mouseover);
  updateForestColors();
}


void wndForestItem::setHighlighted(BOOL highlight) {
  highlight = !!highlight;
  if (highlighted == highlight) return;
  highlighted = highlight;
  updateForestColors();
  onSetHighlighted(highlighted);
}


void wndForestItem::onSetExpanded() {
  if (isExpanded()) {
    if (btn) *btn = "-";
    positionChildren(0);
  } else {
    if (btn) *btn = "+";
  }

  if (isExpanded()) setHeight(getObjectHeight());
  else setHeight(getCollapseWindowHeight());
  setMarginBottom(getClientHeight() - getCollapseHeight());

  if (getParent()) getParent()->invalidate();
}


BOOL wndForestItem::eraseBackground(HDC hdc) {
  RECT rect;
  GetClientRect(getWnd(), &rect);
  rect.bottom = getCollapseHeight();
  FillRect(hdc, &rect, getBackgroundBrush());
  if (isExpanded()) {
    rect.bottom = getExpandedHeight();
    rect.right = 20;
    FillRect(hdc, &rect, getBackgroundBrush());
  }

  return TRUE;
}


void wndForestItem::onResize(int width, int height) {
  wndForestBase::onResize(width, height);
  if (btn) btn->setY( (getCollapseHeight() - btn->getWindowHeight()) >> 1 );
}


BOOL wndForestItem::onCommand(whkInfo *info) {
  if (info  &&  btn  &&  LOWORD(info->param1) == btn->getId()) {
    setExpanded(!isExpanded());
    return TRUE;
  }
  return whkCommand::onCommand(info);
}


#endif
