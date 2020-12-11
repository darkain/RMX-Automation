/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WHK_BASE_H_
#define _WHK_BASE_H_


#include "../wnd/wndBase.h"


typedef struct {
  wndBase *wnd;
  UINT     msg;
  VUINT    param1;
  VUINT    param2;
  VUINT    ret;
} whkInfo;


class whkBase {
  friend class wndBase;
  friend class wndDialog;
  friend class wndSubclass;

  public:
    whkBase(wndBase *wnd=NULL);
    virtual ~whkBase();

  public:
    void insertHookWnd(wndBase *wnd, wndBase *insert=NULL);
    void removeHookWnd(wndBase *wnd);
    HWND getHookWnd() const;

    inline int getHookWndCount() const { return parents.getItemCount(); }

    inline wndBase *getFirstHookWnd()             const { return parents.getFirstItem();    }
    inline wndBase *getNextHookWnd(wndBase *prev) const { return parents.getNextItem(prev); }

    inline BOOL getReturnDefault()   const { return returnDefault; }
    inline void setReturnDefault(BOOL ret) { returnDefault = ret;  }

    inline BOOL hasHooks() const { return parents.hasItems(); }

  protected:
    virtual void onInsertWnd(wndBase *wnd) {}
    virtual void onDeleteWnd(wndBase *wnd);
    virtual BOOL wndHookProc(whkInfo *info) { return FALSE; }

  private:
    BOOL returnDefault;
    rmxList<wndBase*> parents;
};


#endif //_WHK_BASE_H_
