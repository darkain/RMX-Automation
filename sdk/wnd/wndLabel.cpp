/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "wndLabel.h"



wndLabel::wndLabel(wndBase *parent) : wndBase("wndLabel", parent) {
  setBackgroundBrush(NULL);
//  setCreateStyle(WND_REGISTERCLASS);
  setTextColor(GetSysColor(COLOR_WINDOWTEXT));
  transparent = FALSE;
  margin.left = 3;
  margin.top = 0;
  margin.right = 0;
  margin.bottom = 0;
  align.h = TEXTHALIGN_LEFT;
  align.v = TEXTVALIGN_TOP;
}


wndLabel::~wndLabel() {
}


void wndLabel::onNameChange(const char *newname) {
  wndBase::onNameChange(newname);

  if (getTransparent()) {
    wndBase *par = getvParent();
    if (par) par->invalidate(NULL, TRUE);
  }

  invalidate(NULL, TRUE);
}


void wndLabel::setMarginLeft(int left) {
  margin.left = left;
  invalidate();
}

void wndLabel::setMarginRight(int right) {
  margin.right = right;
  invalidate();
}

void wndLabel::setMarginTop(int top) {
  margin.top = top;
  invalidate();
}

void wndLabel::setMarginBottom(int bottom) {
  margin.bottom = bottom;
  invalidate();
}

void wndLabel::setMargins(RECT margins) {
  margin.left = margins.left;
  margin.top = margins.top;
  margin.right = margins.right;
  margin.bottom = margins.bottom;
  invalidate();
}

void wndLabel::setMargins(int left, int top, int right, int bottom) {
  margin.left = left;
  margin.top = top;
  margin.right = right;
  margin.bottom = bottom;
  invalidate();
}


void wndLabel::setTextHAlign(TextHAlign texthalign) {
  align.h = texthalign;
  invalidate();
}

void wndLabel::setTextVAlign(TextVAlign textvalign) {
  align.v = textvalign;
  invalidate();
}

void wndLabel::setTextAlign(TextAlign textalign) {
  align.h = textalign.h;
  align.v = textalign.v;
  invalidate();
}

void wndLabel::setTextAlign(TextHAlign texthalign, TextVAlign textvalign) {
  align.h = texthalign;
  align.v = textvalign;
  invalidate();
}

void wndLabel::setTextColor(COLORREF color, BOOL redraw) {
  textcolor = color;
  if (redraw) invalidate();
}


BOOL wndLabel::eraseBackground(HDC hdc) {
  RECT r;
  getClientRect(&r);
  if (getTransparent()) {
    FillRect(hdc, &r, (HBRUSH)GetStockObject(HOLLOW_BRUSH));
  } else {
    FillRect(hdc, &r, getBackgroundBrush());
  }
  return TRUE;
}


void wndLabel::onPaint(HDC hdc, PAINTSTRUCT *ps) {
  wndBase::onPaint(hdc, ps);

  dc = hdc;
  HGDIOBJ font = SelectObject(hdc, getFont());
  SetBkMode(hdc, TRANSPARENT);
  drawText(*this);
  DeleteObject(font);
  dc = NULL;
}

/*
void wndLabel::drawText(const char *text, RECT *rect) {
  DrawText(dc, text, -1, rect, 0);
}
*/

void wndLabel::drawText(const char *text) {
  if (!text) return;

  rmxUtf8ToWide str(text);

  SIZE size;
  int len = str.getLength();
  GetTextExtentPointW(dc, str.getBuffer(), len, &size);

  int left   = margin.left;
  int top    = margin.top;
  int width  = getClientWidth()  - margin.right  - margin.left;
  int height = getClientHeight() - margin.bottom - margin.top;

  switch (align.h) {
    case TEXTHALIGN_LEFT:   left += 0;                         break;
    case TEXTHALIGN_CENTER: left += (width>>1) - (size.cx>>1); break;
    case TEXTHALIGN_RIGHT:  left += width - size.cx;           break;
  }

  switch (align.v) {
    case TEXTVALIGN_TOP:    top += 0;                          break;
    case TEXTVALIGN_MIDDLE: top += (height>>1) - (size.cy>>1); break;
    case TEXTVALIGN_BOTTOM: top += height - size.cy;           break;
  }

  RECT rect;
  rect.left   = max(left, margin.left);
  rect.top    = max(top,  margin.top );
  rect.right  = (left < margin.left) ? margin.left  + width  : (left + width );
  rect.bottom = (top  < margin.top ) ? margin.right + height : (top  + height);
  
  SetTextColor(dc, textcolor);
  DrawTextW(dc, str.getBuffer(), -1, &rect, 0);
}


void wndLabel::setTransparent(BOOL trans) {
  trans = !!trans;
  if (trans == transparent) return;
  transparent = trans;
//  setMouseParent(transparent);
  invalidate();
}


/*
void wndBase::setFont(const char *name, int size) {
  HDC testhdc = GetDC(NULL);
  long lfHeight = -MulDiv(size, GetDeviceCaps(testhdc, LOGPIXELSY), 72);
  ReleaseDC(NULL, testhdc);

  HFONT hf = CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, name);

  if(hf) {
    HFONT hfOld = (HFONT)SelectObject(hdc, hf);
  }
}
*/


#endif
