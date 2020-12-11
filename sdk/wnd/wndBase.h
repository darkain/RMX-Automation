/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_BASE_H_
#define _WND_BASE_H_


#include "../Win32/MessageWnd.h"


class wndSubclass;
class wndDialog;
class whkBase;


#ifndef SetWindowLongPtr
#define SetWindowLongPtr SetWindowLong
#endif

#ifndef GetWindowLongPtr
#define GetWindowLongPtr GetWindowLong
#endif

#ifndef LONG_PTR
#define LONG_PTR LONG
#endif


class wndBase : public vMessageWnd {
  friend class wndSubclass;
  friend class wndDialog;
  friend class whkBase;

  private:
    void defaultVal();

  public:
    wndBase(const char *classname, wndBase *parent=NULL);
    wndBase(const wchar_t *classname, wndBase *parent=NULL);
    virtual ~wndBase();

  public:
    virtual HWND onCreateWindow();
    virtual void onInit();
    virtual void onPostInit();
    void initChildren(BOOL recursive=FALSE);
    inline BOOL isInited() const { return flags.inited; }
    inline BOOL isPostInited() const { return flags.postinited; }

    virtual void onSetFocus() {}
    virtual void onLoseFocus() {}


    //return TRUE to delete class, or return FALSE to cancel deletion
    virtual BOOL onClose() { return FALSE; }
    void closeWnd(BOOL passive=FALSE);

    void setWnd(HWND wnd) { hwnd = wnd; }

    void setLong(int index, LONG_PTR value) { SetWindowLongPtr(getWnd(), index, value); }
    LONG_PTR getLong(int index) const { return GetWindowLongPtr(getWnd(), index); }

    void setAlwaysOnTop(BOOL ontop);
    void setVisible(BOOL vis);
    BOOL isVisible() const;
    BOOL isParentsVisible() const;
    void hideWindow() { setVisible(FALSE); }
    void showWindow() { setVisible(TRUE);  }
    void hideChildren(BOOL recursive=FALSE);
    void showChildren(BOOL recursive=FALSE);
    virtual void onSetVisible(BOOL vis) {}

    void setEnabled(BOOL en) { EnableWindow(getWnd(), !!en); }
    BOOL isEnabled() const { return IsWindowEnabled(getWnd()); }
    void fakeEnable() { sendMessage(WM_ENABLE, isEnabled()); }


    //window placement functions
    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    int getClientWidth() const;
    int getClientHeight() const;
    int getWindowWidth() const;
    int getWindowHeight() const;
    void getWindowRect(RECT *r) const { GetWindowRect(getWnd(), r); }
    void getClientRect(RECT *r) const { GetClientRect(getWnd(), r); }
    void setX(int x, BOOL redraw=TRUE);
    void setY(int y, BOOL redraw=TRUE);
    void setWidth(int w, BOOL redraw=TRUE);
    void setHeight(int h, BOOL redraw=TRUE);
    void fakeResize();
    void resize(int w, int h, BOOL redraw=TRUE);
    void resizeClient(int w, int h, BOOL redraw=TRUE);
    void move(int x, int y, BOOL redraw=TRUE);
    void moveresize(int x, int y, int w, int h, BOOL redraw=TRUE);
    virtual void onMove(int x, int y) {}
    virtual void onResize(int width, int height) {}
    void updateWindowPosition();

    void setForeground() const { SetForegroundWindow(getWnd()); }
    void setFocus() const { SetFocus(getWnd()); }

    inline BOOL isMinimized() const { return IsIconic(getWnd()); }
    inline BOOL isMaximized() const { return IsZoomed(getWnd()); }
    inline void restoreWindow() { ShowWindow(getWnd(), SW_RESTORE); }
    inline void minimizeWindow() { ShowWindow(getWnd(), SW_MINIMIZE); }
    inline void maximizeWindow() { ShowWindow(getWnd(), SW_MAXIMIZE); }
    //end window placement functions


    //painting functions
    //nothing in this group shall ever become 'const', as it messes up painting
    virtual void onPaint(HDC hdc, PAINTSTRUCT *ps) {}
    virtual BOOL customPaint() { return FALSE; }
    virtual BOOL eraseBackground(HDC hdc) { return FALSE; }
    void updateWindow() { UpdateWindow(getWnd()); }

    void invalidate(RECT *rect=NULL, BOOL bgErase=FALSE);

    //device context
    inline HDC getDC()             const { return GetDC(getWnd()); }
    inline void releaseDC(HDC hdc) const { ReleaseDC(getWnd(), hdc) ;}

    void setBackgroundBrush(HBRUSH brush, BOOL repaint=TRUE);
    HBRUSH getBackgroundBrush() const;
    //end painting functions

    
    //keyboard functions
    virtual void onEnter() {}
    //end keyboard functions


    //parent-child functions
    inline HWND gethParent() const { return (parent) ? parent->getWnd() : NULL; }
    inline wndBase *getvParent() const { return static_cast<wndBase*>(parent); }
    void setvParent(wndBase *wnd);
    void appendChild(wndBase *child);
    void removeChild(wndBase *child, BOOL kill=TRUE);
    inline wndBase *getFirstChild() const { return children.getFirstItem(); }
    inline wndBase *getNextChild(wndBase *c) const { return children.getNextItem(c); }
    wndBase *getChild(HWND wnd);
    //end parent-child functions


    void setIcon(HICON hIcon);
    void setIcon(WORD resource);
    HICON getIcon() { return hicon; }
    virtual void onSetIcon(HICON icon) {}


    //window style information
    void setWindowStyle(  UINT newstyle);
    void setWindowStyleEx(UINT newstyle);
    UINT getWindowStyle()   const;
    UINT getWindowStyleEx() const;
    void appendWindowStyle(  UINT newstyle) { setWindowStyle(  getWindowStyle()   | newstyle); }
    void appendWindowStyleEx(UINT newstyle) { setWindowStyleEx(getWindowStyleEx() | newstyle); }
    void removeWindowStyle(  UINT oldstyle) { setWindowStyle(  getWindowStyle()   & (0xFFFFFFFF ^ oldstyle)); }
    void removeWindowStyleEx(UINT oldstyle) { setWindowStyleEx(getWindowStyleEx() & (0xFFFFFFFF ^ oldstyle)); }
    //end window style information


    void updateNameFromWnd();
    void getWindowText(char *buffer, int length) const;
    int getWindowTextLength() const { return GetWindowTextLength(getWnd()); }
    inline void setWindowName(const char *name) { *this = name; }
    inline const char *getWindowName() const { return *this; }
    virtual void onNameChange(const char *newname);

    void setFont(const char *name, int size);
    void setFont(HFONT font, BOOL redraw=TRUE);
    HFONT getFont() const { return hfont; }

    void setId(UINT newid);
    VUINT getId() const;

    void setRegisterClass(BOOL reg) { flags.reigserclass = !!reg; }

    
    void setTooltip(const char *tip);


    //alpha blended windows - requres Windows 2000 or higher
    void setAlpha(int alpha);  //0=transparent,  255=opaque
    //end alpha blending stuff


    virtual void onTimer(DWORD id, DWORD sysTime) {}
//    virtual UINT onCommand(USHORT id, USHORT cmd, LPARAM lParam) { return 0; }
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);

  private:
    void addWndHook(whkBase *hook) { hooks.appendItem(hook); }
    void delWndHook(whkBase *hook) { hooks.removeItem(hook); }

  protected:
    //do NOT call these directly, use the window sizing functions listed above
    void updateMoveRect(int x, int y, BOOL update=FALSE, BOOL redraw=FALSE);
    void updateSizeRect(int w, int h, BOOL update=FALSE, BOOL redraw=FALSE);
    void updateMoveSizeRect(int x, int y, int w, int h, BOOL update=FALSE, BOOL redraw=FALSE);


  private:
    BOOL paintevent();

  private:
    typedef union {
      DWORD flags;
      struct {
        unsigned inited        :1;
        unsigned postinited    :1;
        unsigned reigserclass  :1;
        unsigned deleting      :1;
        unsigned reserved      :28;
      };
    } FLAGS;

  private:
    FLAGS flags;

    mutable UINT style;
    mutable UINT styleex;
    mutable HFONT hfont;

    HICON hicon;
    HBRUSH bgbrush;

    UINT id;
    RECT pos;

    rmxList<whkBase*> hooks;
    rmxList<wndBase*> children;

    HWND htool;
    rmxString tooltip;
};


#endif //_WND_BASE_H_
