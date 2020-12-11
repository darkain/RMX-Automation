/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "../rmxBase.h"
#include "wndForestItems.h"
#include <commctrl.h>


//---------------------------------------------------------------------------------------------------------



vForestButton::vForestButton(wndForestBase *parent, wndForestBase *insert) : wndForestItem(parent, insert) {
  button = new wndSubclass("Button", this);
  button->setBackgroundBrush((HBRUSH)GetStockObject(HOLLOW_BRUSH));
  button->appendWindowStyle(WS_TABSTOP);
  button->setId(301);
  whkMouse::insertHookWnd(button);

  label = new wndLabel(this);
  label->setTransparent(TRUE);
  label->setTextHAlign(TEXTHALIGN_RIGHT);
  label->setTextVAlign(TEXTVALIGN_MIDDLE);
  label->setMarginRight(5);
  label->move(20, 0);
  whkMouse::insertHookWnd(label);
}

vForestButton::~vForestButton() {
  button = NULL;
  label = NULL;
}

void vForestButton::onInit() {
  wndForestItem::onInit();
  button->init();
  label->init();
  button->showWindow();
  label->showWindow();
}


void vForestButton::setDescription(const char *desc) {
	if (label) *label = desc;
}

const char *vForestButton::getDescription() const {
	return label ? *label : NULL;
}

void vForestButton::setButtonText(const char *text) {
	if (button) button = text;
}

const char *vForestButton::getButtonText() const {
	return button ? *button : NULL;
}

void vForestButton::onResize(int width, int height) {
  wndForestItem::onResize(width, height);
//  if (button) button->moveresize(width-102, 0, 100, getCollapseHeight(), FALSE);
//  if (label) label->resize(width-120, getCollapseHeight(), FALSE);
  HDWP group = BeginDeferWindowPos(2);
  DeferWindowPos(group, button->getWnd(), NULL, width-102, 0, 100, getCollapseHeight(), SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOREDRAW);
  DeferWindowPos(group, label->getWnd(), NULL, 0, 0, width-120, getCollapseHeight(), SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOREDRAW);
  EndDeferWindowPos(group);
}

void vForestButton::onSetHighlighted(BOOL highlighted) {
  wndForestItem::onSetHighlighted(highlighted);
  if (label) label->setTextColor(getTextColor());
}

void vForestButton::onSetMouseOver(BOOL over) {
  wndForestItem::onSetMouseOver(over);
  if (label) label->setTextColor(getTextColor());
}

BOOL vForestButton::onCommand(whkInfo *info) {
  if (info) if (button) if (LOWORD(info->param1)==button->getId()) {
    onButtonClick();
    return TRUE;
  }
  return wndForestItem::onCommand(info);
}

void vForestButton::onSetItemEnabled(BOOL en) {
  wndForestItem::onSetItemEnabled(en);
  if (button) button->setEnabled(en);
}


//---------------------------------------------------------------------------------------------------------


vForestBool::vForestBool(wndForestBase *parent, wndForestBase *insert) : wndForestItem(parent, insert) {
  check = new wndSubclass("Button", this);
//  check->setBackgroundBrush((HBRUSH)GetStockObject(HOLLOW_BRUSH));
  check->setBackgroundBrush(getBackgroundBrush());
  check->appendWindowStyle(WS_TABSTOP);
  check->setId(301);
  check->appendWindowStyle(BS_CHECKBOX);
  check->moveresize(0, 0, 18, 18);
  whkMouse::insertHookWnd(check);
}

vForestBool::~vForestBool() {
  check = NULL;
}

void vForestBool::onInit() {
  wndForestItem::onInit();

  check->init();
  check->showWindow();
}

void vForestBool::setChecked(BOOL checked) {
  checked = !!checked;
  if (checked == getChecked()) return;
  if (check) {
    check->sendMessage( BM_SETCHECK, checked );
    onSetChecked(checked);
  }
}

BOOL vForestBool::getChecked() const {
  if (check) return !!check->sendMessage( BM_GETCHECK );
  return FALSE;
}

void vForestBool::onLeftDown(int x, int y) {
  wndForestItem::onLeftDown(x, y);
  if (check) if (check->isEnabled()) {
    //do stuff
  }
}

void vForestBool::onResize(int width, int height) {
  wndForestItem::onResize(width, height);
  if (check) check->move(width-16, 1, FALSE);
}


BOOL vForestBool::onCommand(whkInfo *info) {
  if (info) if (check) if (LOWORD(info->param1)==check->getId()) {
    setChecked( !getChecked() );
    return TRUE;
  }
  return wndForestItem::onCommand(info);
}
/*
UINT vForestBool::onCommand(USHORT id, USHORT cmd, LPARAM lParam) {
  if (cmd == BN_CLICKED) if (check) if (id == check->getId()) {
    setChecked( !getChecked() );
    return 0;
  }
  return wndForestItem::onCommand(id, cmd, lParam);
}
*/

void vForestBool::onSetItemEnabled(BOOL en) {
  wndForestItem::onSetItemEnabled(en);
  if (check) check->setEnabled(en);
}

void vForestBool::onSetHighlighted(BOOL highlighted) {
  wndForestItem::onSetHighlighted(highlighted);
  check->setBackgroundBrush(getBackgroundBrush());
}

void vForestBool::onSetMouseOver(BOOL over) {
  wndForestItem::onSetMouseOver(over);
  check->setBackgroundBrush(getBackgroundBrush());
}


//---------------------------------------------------------------------------------------------------------


vForestSlider::vForestSlider(wndForestBase *parent, wndForestBase *insert) : wndForestItem(parent, insert) {
  slider = new wndSubclass(TRACKBAR_CLASS, this);
  slider->setBackgroundBrush(getBackgroundBrush());
  slider->appendWindowStyle(WS_TABSTOP);
  whkMouse::insertHookWnd(this);
  whkMouse::insertHookWnd(slider);
  whkTrack::insertHookWnd(slider);
  whkTrack::insertHookWnd(this);
  setCollapseHeight(30);
}

vForestSlider::~vForestSlider() {
  slider = NULL;
}

void vForestSlider::onInit() {
  wndForestItem::onInit();

  slider->init();
  slider->sendMessage( TBM_SETTIC, 0, 0 );
  slider->showWindow();
}

void vForestSlider::onResize(int width, int height) {
  wndForestItem::onResize(width, height);
  if (slider) slider->moveresize(width>>1, 0, width>>1, getCollapseHeight(), FALSE);
}

void vForestSlider::onSetHighlighted(BOOL highlighted) {
  wndForestItem::onSetHighlighted(highlighted);
  if (!slider) return;
  slider->setBackgroundBrush(getBackgroundBrush(), FALSE);
  slider->fakeEnable();
}

void vForestSlider::onSetMouseOver(BOOL over) {
  wndForestItem::onSetMouseOver(over);
  if (!slider) return;
  slider->setBackgroundBrush(getBackgroundBrush(), FALSE);
  slider->fakeEnable();
}


void vForestSlider::onSetItemEnabled(BOOL en) {
  wndForestItem::onSetItemEnabled(en);
  if (slider) slider->setEnabled(en);
}


//---------------------------------------------------------------------------------------------------------


vForestEdit::vForestEdit(wndForestBase *parent, wndForestBase *insert) : wndForestItem(parent, insert) {
  edit = new vForestEditWnd(this);
  edit->appendWindowStyle(WS_TABSTOP | WS_BORDER);
  edit->moveresize(0, 0, 100, 19);
  whkMouse::insertHookWnd(edit);
}

vForestEdit::~vForestEdit() {
  edit = NULL;
}

void vForestEdit::onInit() {
  wndForestItem::onInit();
  edit->init();
  edit->showWindow();
}

void vForestEdit::onResize(int width, int height) {
  wndForestItem::onResize(width, height);
  int w = width >> 1;
  if (edit) edit->moveresize(w, 0, width-w, getCollapseHeight(), FALSE);
}

void vForestEdit::setEditText(const char *text) {
  if (edit) edit = text;
}

const char *vForestEdit::getEditText() const {
  if (!edit) return NULL;
  edit->updateNameFromWnd();
  return *edit;
}


void vForestEdit::onSetItemEnabled(BOOL en) {
  wndForestItem::onSetItemEnabled(en);
  if (edit) {
    if (edit->isInited()) {
      edit->sendMessage(EM_SETREADONLY, !en);
    } else {
      if (!en) edit->appendWindowStyle(ES_READONLY);
      else edit->removeWindowStyle(ES_READONLY);
    }
  }
}


//---------------------------------------------------------------------------------------------------------


#endif
