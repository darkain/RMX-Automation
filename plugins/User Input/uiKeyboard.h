/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _UI_KEYBOARD_H_
#define _UI_KEYBOARD_H_


#include "../../sdk/rmxLock.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/cfg/cfgAutoList.h"
#include "../../sdk/Win32/MessageWnd.h"
#include "uiKeyboardEvent.h"


class uiKeyboardTree;
#define uiKeyboardParent tcfgAutoEventList<cfgTree, uiKeyboardEvent>


class uiKeyboard : public uiKeyboardParent, public rmxLock {
  public:
    uiKeyboard(cfgBase *parent);
    virtual ~uiKeyboard();

  public:
    void setActive(uiKeyboardEvent *event) { active = event; }

    HHOOK getHook() const { return hook; }

    void translateCodes(DWORD vkCode, DWORD scanCode, DWORD keyFlags, char *str);
    BOOL hookmsg(WPARAM wParam, DWORD vkCode, DWORD scanCode=0, DWORD keyFlags=0);

    uiKeyboardEvent *getEvent(const char *raw);


  private:
    static LRESULT CALLBACK winnt_hook (int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK win9x_hook (int nCode, WPARAM wParam, LPARAM lParam);


  private:
    uiKeyboardEvent *active;

    vMessageWnd *wnd;
    HHOOK        hook;
};


extern uiKeyboard *keyboard;



class uiKeyboardWnd : public vMessageWnd {
  public:
    uiKeyboardWnd() : vMessageWnd("Keyboard Shortcuts") {}
    virtual ~uiKeyboardWnd() {}
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);
};


#endif //_UI_KEYBOARD_H_
