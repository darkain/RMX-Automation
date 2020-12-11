/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


/***********************************************************************\
* this file contains a bunch of pre-made helper classes for cfgBase     *
* - none of these classes directly rely on UserData                     *
*                                                                       *
* these classes are specifically designed to work with wndBase          *
\***********************************************************************/


#ifndef _CFG_WND_HELPER_H_
#define _CFG_WND_HELPER_H_


#include "Config.h"
#include "../cb/cbBase.h"
#include "../rmxLock.h"
#include "../wnd/wndBase.h"
#include "../whk/whkBase.h"



template <class T>
class cfgWndPos : public T, public whkBase/*, public cbBase*/, public rmxLock {
  public:
    cfgWndPos(const char *name, cfgBase *list, wndBase *window=NULL)
      : T(name, list), whkBase(window) {
      lock();
      x = new cfgInt("X", this);
      y = new cfgInt("Y", this);
      x->insertCallback(this);
      y->insertCallback(this);
      unlock();
    }


    virtual ~cfgWndPos() {
      lock();
      x->removeCallback(this);
      y->removeCallback(this);
    }


  protected:
    virtual BOOL wndHookProc(whkInfo *info) {
      wndBase *wnd = getFirstHookWnd();
      if (!wnd  ||  wnd->isMinimized()  ||  wnd->isMaximized()) return whkBase::wndHookProc(info);
      if (isUnlocked()  &&  info->msg == WM_WINDOWPOSCHANGED) {
        lock();
        WINDOWPOS *pos = (WINDOWPOS*)info->param2;
        if (!(pos->flags & SWP_NOMOVE)) {
          x->setValue( pos->x  );
          y->setValue( pos->y  );
        }
        unlock();
      }
      return whkBase::wndHookProc(info);
    }


    virtual void cb_onSetUserData(const prmBase *userdata) {
//TODO: FIX THIS SHIT!!
//      cbBase::cb_onSetUserData(userdata);
      if (isUnlocked()) {
        lock();
        wndBase *wnd = getFirstHookWnd();
        if (wnd) wnd->move(x->getValue(), y->getValue());
        unlock();
      }
    }


    virtual void onInsertWnd(wndBase *wnd) {
      whkBase::onInsertWnd(wnd);
      if (isUnlocked()) {
        lock();
        if (wnd) wnd->move(x->getValue(), y->getValue());
        unlock();
      }
    }

    
    virtual void cb_onDelete() {}  //do nothing

  private:
    cfgInt *x;
    cfgInt *y;
};


//-------------------------------------------------------------------------------------------------


//TODO: FIX THIS SHIT!!
template <class T>
class cfgWndSizePos : public T, public whkBase/*, public cbBase*/, public rmxLock {
  public:
    cfgWndSizePos(const char *name, cfgBase *parent, wndBase *window=NULL)
      : T(name, parent), whkBase(window) {
      lock();
        x = new cfgInt("X",      this);
        y = new cfgInt("Y",      this);
        w = new cfgInt("Width",  this);
        h = new cfgInt("Height", this);
        x->insertCallback(this);
        y->insertCallback(this);
        w->insertCallback(this);
        h->insertCallback(this);
        w->setValue(600, true);
        h->setValue(400, true);
      unlock();
    }


    virtual ~cfgWndSizePos() {
      lock();
      x->removeCallback(this);
      y->removeCallback(this);
      w->removeCallback(this);
      h->removeCallback(this);
    }


  protected:
    virtual BOOL wndHookProc(whkInfo *info) {
      wndBase *wnd = getFirstHookWnd();
      if (!wnd  ||  wnd->isMinimized()  ||  wnd->isMaximized()) return whkBase::wndHookProc(info);
      if (isUnlocked()  &&  info->msg == WM_WINDOWPOSCHANGED) {
        lock();
        WINDOWPOS *pos = (WINDOWPOS*)info->param2;
        if (!(pos->flags & SWP_NOMOVE)) {
          x->setValue( pos->x  );
          y->setValue( pos->y  );
        }
        if (!(pos->flags & SWP_NOSIZE)) {
          w->setValue( pos->cx );
          h->setValue( pos->cy );
        }
        unlock();
      }
      return whkBase::wndHookProc(info);
    }


    virtual void cb_onSetUserData(const prmBase *userdata) {
//TODO: FIX THIS SHIT!!
//      cbBase::cb_onSetUserData(userdata);
      if (isUnlocked()) {
        lock();
        wndBase *wnd = getFirstHookWnd();
        if (wnd) wnd->moveresize(x->getValue(), y->getValue(), w->getValue(), h->getValue());
        unlock();
      }
    }


    virtual void onInsertWnd(wndBase *wnd) {
      whkBase::onInsertWnd(wnd);
      if (isUnlocked()) {
        lock();
        if (wnd) wnd->moveresize(x->getValue(), y->getValue(), w->getValue(), h->getValue());
        unlock();
      }
    }


    virtual void cb_onDelete() {}  //do nothing

  private:
    cfgInt *x;
    cfgInt *y;
    cfgInt *w;
    cfgInt *h;
};


#endif //_CFG_WND_HELPER_H_
