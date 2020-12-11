/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_BUTTON_H_
#define _WND_BUTTON_H_


#include "wndSubclass.h"


class wndButton : public wndSubclass {
  public:
    wndButton(const char *text, wndBase *parent);
    virtual ~wndButton();
};


class wndButtonResize : public wndButton {
  public:
    wndButtonResize(const char *text, wndBase *parent);
    virtual ~wndButtonResize();

  public:
    void autosize();

  protected:
    virtual void onInit();
    virtual void onNameChange(const char *newname);
};


#endif //_WND_BUTTON_H_
