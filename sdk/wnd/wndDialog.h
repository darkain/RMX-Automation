/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_DIALOG_H_
#define _WND_DIALOG_H_


#include "wndBase.h"


class wndDialog : public wndBase {
  public:
    wndDialog(WORD resource, wndBase *parent=NULL);
    virtual ~wndDialog();

  public:
    inline HWND getDialogItem(int resource) const { return GetDlgItem(getWnd(), resource); }
    void setDialogItemText(int resource, const char *text);

  protected:
    virtual HWND onCreateWindow();
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);

  private:
    WORD dlg;
};


#endif //_WND_DIALOG_H_
