/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#if 0

#include "../rmxBase.h"
#include "wndButton.h"


wndButton::wndButton(const char *text, wndBase *parent) 
 : wndSubclass("button", parent) {

   *this = text;
}


wndButton::~wndButton() {
}


//-------------------------------------------------------------------------------------


wndButtonResize::wndButtonResize(const char *text, wndBase *parent)
 : wndButton(text, parent) {
}


wndButtonResize::~wndButtonResize() {
}


void wndButtonResize::autosize() {
  SIZE size = {0, 0};
  HDC  hdc  = getDC();

  rmxUtf8ToWide string(*this);

  GetTextExtentPoint(hdc, string.getBuffer(), string.getLength(), &size);

  resizeClient(size.cx + 10, size.cy + 4);

  releaseDC(hdc);  //icky bad possibly?
}


void wndButtonResize::onInit() {
  wndButton::onInit();
  autosize();
}


void wndButtonResize::onNameChange(const char *newname) {
  wndButton::onNameChange(newname);
  autosize();
}


#endif
