/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _TRAYICON_H_
#define _TRAYICON_H_


#include "MessageWnd.h"
#include <shellapi.h>


class vTrayIcon : public vMessageWnd {
  public:
    vTrayIcon(HICON icon=NULL);
    virtual ~vTrayIcon();

    void popBallonTip(const char *title, const char *msg, UINT timeout=5000);
    
    void setVisible(BOOL vis);
    inline BOOL isVisible() { return visible; }

    void setIcon(HICON icon, BOOL destroy=TRUE);
    void setIcon(const char *filename);
    void setIcon(WORD resource);
    inline HICON getIcon() const { return hicon; }

  protected:
    virtual void onInit();

    virtual void onNameChange(const char *newname);

    virtual void onLeftDown(int x, int y) {}
    virtual void onLeftUp(int x, int y) {}
    virtual void onLeftClick(int x, int y) {}
    virtual void onLeftDoubleClick(int x, int y) {}

    virtual void onMiddleDown(int x, int y) {}
    virtual void onMiddleUp(int x, int y) {}
    virtual void onMiddleClick(int x, int y) {}
    virtual void onMiddleDoubleClick(int x, int y) {}

    virtual void onRightDown(int x, int y) {}
    virtual void onRightUp(int x, int y) {}
    virtual void onRightClick(int x, int y) {}
    virtual void onRightDoubleClick(int x, int y) {}

    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);

  private:
    void updateIcon(BOOL add=FALSE);
    void removeIcon();

  private:
    HICON hicon;
    NOTIFYICONDATA icond;
    BOOL visible;

    static UINT taskbarmsg;
};


#endif //_TRAYICON_H_
