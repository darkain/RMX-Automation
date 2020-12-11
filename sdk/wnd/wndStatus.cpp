/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "wndStatus.h"
#include "wndButton.h"
#include <commctrl.h>


wndStatus::wndStatus(wndBase *parent) : wndSubclass(STATUSCLASSNAMEA, parent) {
  appendWindowStyle(SBARS_SIZEGRIP | SBARS_TOOLTIPS);
}


wndStatus::~wndStatus() {
}

void wndStatus::autoposition() {
  sendMessage(WM_SIZE);
}

void wndStatus::setStatusParts(int *array, int numparts) {
  sendMessage(SB_SETPARTS, numparts, (VUINT)array);
}

void wndStatus::setStatusText(const char *text, int bar) {
  rmxUtf8ToWide str(text);
  sendMessage(SB_SETTEXT, bar, (VUINT)str.getBuffer());
}

char *wndStatus::getStatusText(char *buffer, int bar) {
  //todo: UNICODE
  sendMessageA(SB_GETTEXT, bar, (VUINT)buffer);
  return buffer;
}

VINT wndStatus::getStatusTextLength(int bar) {
  return sendMessage(SB_GETTEXTLENGTH, bar);
}

void wndStatus::getStatusRect(RECT *r, int bar) {
  sendMessage(SB_GETRECT, bar, (VUINT)r);
}
/*
UINT wndStatus::onCommand(USHORT id, USHORT cmd, LPARAM lParam) {
  wndBase *par = getvParent();
  if (par) return par->onCommand(id, cmd, lParam);
  return vSubclassWnd::onCommand(id, cmd, lParam);
}
*/



//----------------------------------------------------------------------------------------



vButtonStatusWnd::vButtonStatusWnd(wndBase *parent) : wndStatus(parent) {
  appendWindowStyle(WS_CLIPCHILDREN);
}


vButtonStatusWnd::~vButtonStatusWnd() {
}


void vButtonStatusWnd::onResize(int width, int height) {
  wndStatus::onResize(width, height);

  int sbborders[] = {0, 0, 0};
  sendMessage(SB_GETBORDERS, 0, (LPARAM)sbborders);

  int top  = sbborders[1] / 2;
  int left = sbborders[1] / 2;
  int size = getClientHeight() / 2;

  int parts = 1;
  if (leftButtons.hasItems())  parts++;
  if (rightButtons.hasItems()) parts++;

  int textPart = (leftButtons.hasItems()) ? (1) : (0);


  int leftwidth = left;
  wndBase *item = leftButtons.getFirstItem();
  while (item) {
    item->move(leftwidth, size - (item->getHeight() / 2));
    leftwidth += item->getWidth();
    item = leftButtons.getNextItem(item);
  }
  leftwidth += left;


  int rightwidth = 0;
  item = rightButtons.getFirstItem();
  while (item) {
    item->setX(rightwidth);
    rightwidth += item->getWidth();
    item = rightButtons.getNextItem(item);
  }


  int middlewidth = (getClientWidth() - leftwidth) - rightwidth;

  int widths[3] = {0, 0, 0}; //{100+sbborders[2], width};
  widths[0] = ((textPart == 1) ? ( leftwidth ) : (middlewidth)) + 0;
  widths[1] = ((textPart == 1) ? (middlewidth) : (rightwidth )) + widths[0];
  widths[2] = (rightwidth) + widths[1];

  setStatusParts(widths, parts);
}


wndBase *vButtonStatusWnd::addButton(const char *text, int id, vStatusSide side) {
  if (getButton(id)) return NULL;

  wndBase *button = new wndButtonResize(text, this);
  button->setId(id);
  button->appendWindowStyle(WS_VISIBLE);

  if (side == STATUS_LEFT) {
    leftButtons.appendItem(button);
  } else {
    rightButtons.appendItem(button);
  }

  if (isInited()) {
    button->init();
  }

  fakeResize();
  return button;
}


wndBase *vButtonStatusWnd::getButton(int id) {
  wndBase *wnd = leftButtons.getFirstItem();
  while (wnd) {
    if (wnd->getId() == id) return wnd;
    wnd = leftButtons.getNextItem(wnd);
  }

  wnd = rightButtons.getFirstItem();
  while (wnd) {
    if (wnd->getId() == id) return wnd;
    wnd = rightButtons.getNextItem(wnd);
  }

  return NULL;
}


void vButtonStatusWnd::removeButton(int id) {
  wndBase *wnd = leftButtons.getFirstItem();
  while (wnd) {
    if (wnd->getId() == id) {
      delete wnd;
      leftButtons.removeItem(wnd);
      return;
    }
    wnd = leftButtons.getNextItem(wnd);
  }

  wnd = rightButtons.getFirstItem();
  while (wnd) {
    if (wnd->getId() == id) {
      delete wnd;
      leftButtons.removeItem(wnd);
      return;
    }
    wnd = rightButtons.getNextItem(wnd);
  }
}


#endif
