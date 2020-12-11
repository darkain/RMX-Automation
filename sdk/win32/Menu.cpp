/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "Menu.h"


short vMenu::menuid = 1;



vMenu::vMenu() {
  hMenu = CreatePopupMenu();
//  hTmp = NULL;
  parent = NULL;
}


vMenu::vMenu(const char *name, vMenu *par) : rmxString(name) {
//  hTmp = NULL;
  parent = par;
  hMenu = NULL;
  myid = parent->addMenuItem(get(), (UINT)this);
  parent->appendMenu(this);
}


vMenu::vMenu(vMenu *par, int sep) {
  hMenu = NULL;
//  hTmp = NULL;
  parent = par;

  myid = 0;

  MENUITEMINFO mi;
  ZeroMemory(&mi, sizeof(MENUITEMINFO));
  mi.cbSize = sizeof(MENUITEMINFO);
  mi.fMask = MIIM_TYPE;
  mi.fType = MFT_SEPARATOR;
  InsertMenuItem(par->getMenu(), -1, TRUE, &mi);
}

/*
vMenu::vMenu(vMenu* par, const char* name) {
  vMenu();
  parent = par;
  parent->appendMenu(this, name);
}
*/

/*
vMenu::vMenu(HMENU menu) {
  hMenu = menu;
  parent = NULL;
  hTmp = NULL;
}

/*
vMenu::vMenu(HINSTANCE hInst, DWORD resource, int subItem) {
  hTmp = LoadMenu(hInst, MAKEINTRESOURCE(resource));
  if (subItem == -1) hMenu = hTmp;
  else hMenu = GetSubMenu(hTmp, subItem);
  parent = NULL;
}
*/


vMenu::~vMenu() {
  if (parent) parent->removeMenu(this);

  vMenu *menu = children.getFirstItem();
  while (menu) {
    delete menu;
    children.removeItem(menu);
    menu = children.getFirstItem();
  }

//  if (hTmp)  DestroyMenu(hTmp);
  if (hMenu) DestroyMenu(hMenu);
}



VUINT vMenu::popup(int X, int Y, HWND hWnd) {
  SetForegroundWindow(hWnd);
  int ret = TrackPopupMenuEx(hMenu, TPM_RETURNCMD, X, Y, hWnd, NULL);
  if (RMXOS::isWinNT()) return ret;
  
  MENUITEMINFO info;
  ZeroMemory(&info, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO)-4;
  info.fMask = MIIM_DATA;
  GetMenuItemInfo(hMenu, ret, FALSE, &info);
  return info.dwItemData;
}


VUINT vMenu::popup(HWND hWnd) {
  POINT p;
  GetCursorPos(&p);
  return popup(p.x, p.y, hWnd);
}


HMENU vMenu::findItem(const char *name) {
  int cnt = GetMenuItemCount(hMenu);
  if (cnt == -1) return NULL;

  for (int i=0; i<cnt; i++) {
    wchar_t str[1024] = L"";
    GetMenuString(hMenu, i, str, (sizeof(str)/2)-1, MF_BYPOSITION);

    rmxToUtf8 utfstr(str);
    if (VSTRCMP(name, utfstr.getBuffer()) == 0) return GetSubMenu(hMenu, i);
  }
  return NULL;
}


BOOL vMenu::appendMenu(UINT ID, const char *name, int pos) {
  rmxUtf8ToWide str(name);
  return AppendMenu(hMenu, MF_STRING, ID, str.getBuffer());
}


BOOL vMenu::appendMenu(vMenu *m, const char *name, int pos) {
  if (!m) return FALSE;
  if (!children.hasItem(m)) children.appendItem(m);
  rmxUtf8ToWide str(name);
  return AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)m->getMenu(), str.getBuffer());
}


UINT vMenu::addMenuItem(const char *name, UINT id, BOOL checked) {
  if (!hMenu) {
    hMenu = CreatePopupMenu();
    if (parent) {
      DeleteMenu(parent->getMenu(), myid, MF_BYCOMMAND);
      parent->appendMenu((UINT)this, *this);
    }
  }

  UINT ret = 0;
  rmxUtf8ToWide str(name);

  MENUITEMINFO info;
  ZeroMemory(&info, sizeof(MENUITEMINFO));
  info.cbSize = sizeof(MENUITEMINFO);
  info.fType = MFT_STRING;
  info.dwItemData = id;
  info.dwTypeData = (wchar_t*)str.getBuffer();

  if (RMXOS::isWin50()) {
    info.fMask = MIIM_ID | MIIM_DATA | MIIM_FTYPE | MIIM_STRING;
    info.wID = id;
    InsertMenuItem(hMenu, -1, TRUE, &info);
    ret = id;
  } else  if (RMXOS::isWinNT()) {
    AppendMenu(hMenu, MF_STRING, id, str.getBuffer());
    ret = id;
  } else {
    info.fMask = MIIM_DATA;
    menuid++;
    if (menuid == 0) menuid = 1;
    if (menuid == -1) menuid = 1;
    info.wID = menuid;
    AppendMenu(hMenu, MF_STRING, menuid, str.getBuffer());
    info.cbSize -= 4;
    SetMenuItemInfo(hMenu, menuid, FALSE, &info);
    ret = menuid;
  }

  if (checked) {
    CheckMenuItem(getMenu(), (UINT)info.dwItemData, MF_CHECKED|MF_BYCOMMAND);
  }

  return ret;
}


void vMenu::setChecked(BOOL checked) {
  if (!parent) return;
  if (checked) {
    CheckMenuItem(parent->getMenu(), myid, MF_CHECKED|MF_BYCOMMAND);
  } else {
    CheckMenuItem(parent->getMenu(), myid, MF_UNCHECKED|MF_BYCOMMAND);
  }
}


void vMenu::setEnabled(BOOL enabled) {
  if (!parent) return;
  if (enabled) {
    EnableMenuItem(parent->getMenu(), myid, MF_ENABLED|MF_BYCOMMAND);
  } else {
    EnableMenuItem(parent->getMenu(), myid, MF_GRAYED|MF_BYCOMMAND);
  }
}


vMenuSep::vMenuSep(vMenu *parent) : vMenu(parent, 1) {
}
