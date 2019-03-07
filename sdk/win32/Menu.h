/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __MENU_H_
#define __MENU_H_


#include "Win32.h"
#include "../rmxList.h"
#include "../rmxString.h"


class vMenu : public rmxString {
  public:
    vMenu();
//    vMenu(vMenu* par, const char* name="name");  //DONT USE THIS ONE!!!
    vMenu(const char *name, vMenu *par);  //THIS is the good one
//    vMenu(HMENU menu);
//    vMenu(const char *name);
//    vMenu(HINSTANCE hInst, DWORD resource, int subItem=-1);
    virtual ~vMenu();

   
  protected:
    vMenu(vMenu* par, int sep);  //dont use this, create a vMenuSep instead

  public:
    HMENU getMenu() { return hMenu; }
    HMENU findItem(const char* name);
    HMENU findItem(UINT *ID);

    UINT addMenuItem(const char *name, UINT id, BOOL checked=FALSE);

    BOOL appendMenu(UINT ID, const char* name, int pos=-1);
    BOOL appendMenu(vMenu *m, const char* name, int pos=-1);

    VUINT popup(int X, int Y, HWND hWnd);
    VUINT popup(HWND hWnd);

    void setChecked(BOOL checked);
    void setEnabled(BOOL enabled);

  private:
    void appendMenu(vMenu *menu) { children.appendItem(menu); }
    void removeMenu(vMenu *menu) { children.removeItem(menu); }

  public:
    virtual void onSelect() {}

  protected:
    HMENU hMenu;
//    HMENU hTmp;
    vMenu *parent;
    rmxList<vMenu*> children;

    UINT myid;

    static short menuid;
};


class vMenuSep : public vMenu {
  public:
    vMenuSep(vMenu *parent);
    virtual ~vMenuSep() {}
};


#endif __MENU_H_
