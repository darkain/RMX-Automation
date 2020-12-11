/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_SUBCLASS_H_
#define _WND_SUBCLASS_H_


#include "wndBase.h"


class wndSubclass : public wndBase {
  private:
    void subclassWindow();

  public:
    wndSubclass(const char *classname, wndBase *parent=NULL);
    wndSubclass(const wchar_t *classname, wndBase *parent=NULL);
    virtual ~wndSubclass();

  protected:
    virtual void onInit();

    virtual VUINT subWndProc(UINT msg, VUINT param1, VUINT param2);

  private:
    static LRESULT CALLBACK static_subWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  protected:
    WNDPROC prewndBaseProc;
};


#endif //_WND_SUBCLASS_H_
