/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

#if 0

#include "../rmxBase.h"
#include "wndBase.h"
#include "../whk/whkBase.h"

#include <commctrl.h>


void wndBase::defaultVal() {
  hfont   = NULL;
  hicon   = NULL;
  bgbrush = NULL;
  htool   = NULL;
  
  flags.flags = 0;
  flags.reigserclass = 1;

  style   = 0;
  styleex = 0;

  pos.left   = 0;
  pos.top    = 0;
  pos.right  = 0;
  pos.bottom = 0;

  bgbrush = (HBRUSH)(COLOR_WINDOW);

  id = 0;
  hparent = NULL;
}


wndBase::wndBase(const char *classname, wndBase *parent) : vMessageWnd(classname, parent) {
  //TODO: if no parent is specified, we should generate a top-level window as a "container" for this window ???
  defaultVal();
  if (parent) parent->appendChild(this);
}


wndBase::wndBase(const wchar_t *classname, wndBase *parent) : vMessageWnd(classname, parent) {
  //TODO: if no parent is specified, we should generate a top-level window as a "container" for this window ???
  defaultVal();
  if (parent) parent->appendChild(this);
}


wndBase::~wndBase() { 
  flags.deleting = TRUE;
  setVisible(FALSE);

  if (getvParent()) getvParent()->removeChild(this, FALSE);

  wndBase *wnd = children.getFirstItem();
  while (wnd) {
    removeChild(wnd, TRUE);
    wnd = children.getFirstItem();
  }

  whkBase *hook = hooks.getFirstItem();
  while (hook) {
    hook->onDeleteWnd(this);
    hooks.removeItem(hook);
    hook = hooks.getFirstItem();
  }

  setIcon((HICON)NULL);
  setBackgroundBrush((HBRUSH)NULL);
  setFont((HFONT)NULL);
}


HWND wndBase::onCreateWindow() {
  rmxUtf8ToWide classstr(getClassName());
  rmxUtf8ToWide namestr(*this);

  if (getvParent()) {
    hparent = getvParent()->getWnd();
    getvParent()->appendChild(this);
  }

  HWND hwnd = NULL;

  if (hparent) {
    appendWindowStyle(WS_CHILD);
  } else {
    if (!(getWindowStyle() & WS_POPUP)) {
      appendWindowStyle(WS_OVERLAPPEDWINDOW);
    }
  }

  if (flags.reigserclass) {
    WNDCLASSEX wc;
    if (GetClassInfoEx(RMXOS::getInstance(), classstr.getBuffer(), &wc)) {
    } else if (GetClassInfoEx(NULL, classstr.getBuffer(), &wc)) {
    } else {
      ZeroMemory(&wc, sizeof(WNDCLASSEX));
      wc.cbSize        = sizeof(WNDCLASSEX);
      wc.lpfnWndProc   = (WNDPROC)vMessageWnd::static_wndProc;
      wc.hInstance     = RMXOS::getInstance();
      wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
      wc.hbrBackground = (HBRUSH)GetStockObject(HOLLOW_BRUSH);//bgbrush;
      wc.lpszClassName = classstr.getBuffer();
      wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
      wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
      if (RegisterClassEx(&wc) == NULL) {
        MessageBox(MB_OK, L"Error while registering Window Class", L"wndBase:init()", MB_ICONWARNING);
        return NULL;
      }
    }
  }

  hwnd = CreateWindowEx(getWindowStyleEx(), classstr.getBuffer(), namestr.getBuffer(), getWindowStyle(), getX(), getY(), getWidth(), getHeight(), hparent, (HMENU)id, RMXOS::getInstance(), this);
  if (!hwnd) return NULL;

  if (!getParentWnd()) {
    RECT w;
    getWindowRect(&w);
    pos.left   = w.left;
    pos.top    = w.top;
    pos.right  = w.right  - w.left;
    pos.bottom = w.bottom - w.top;
  }

  return hwnd;
}


void wndBase::onInit() {
  setFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
  flags.inited = TRUE;
  initChildren();
}


void wndBase::onPostInit() {
  vMessageWnd::onPostInit();

  flags.postinited = TRUE;
  fakeResize();

  if (!getParentWnd()) {
    RECT w;
    getWindowRect(&w);
    pos.left   = w.left;
    pos.top    = w.top;
    pos.right  = w.right  - w.left;
    pos.bottom = w.bottom - w.top;
  }
}


void wndBase::initChildren(BOOL recursive) {
  wndBase *child = children.getFirstItem();
  while (child) {
    if (!child->isInited()) {
      child->init();
      if (recursive) child->initChildren(recursive);
    }
    child = children.getNextItem(child);
  }
}


void wndBase::setWindowStyle(UINT newstyle) {
  style = newstyle;
  if (getWnd()) {
    setLong(GWL_STYLE, style);
    SetWindowPos(getWnd(), NULL, 0, 0, 0, 0, SWP_FRAMECHANGED);
  }
}

void wndBase::setWindowStyleEx(UINT newstyle) {
  styleex = newstyle;
  if (getWnd()) {
    setLong(GWL_EXSTYLE, styleex);
    SetWindowPos(getWnd(), NULL, 0, 0, 0, 0, SWP_FRAMECHANGED);
  }
}

UINT wndBase::getWindowStyle() const {
  if (getWnd()) style = (UINT)getLong(GWL_STYLE);
  return style;
}

UINT wndBase::getWindowStyleEx() const {
  if (getWnd()) styleex = (UINT)getLong(GWL_EXSTYLE);
  return styleex;
}


void wndBase::setvParent(wndBase *wnd) {
  if (getvParent()) getvParent()->removeChild(this, FALSE);
  parent = wnd;
  if (getvParent()) getvParent()->appendChild(this);
}


void wndBase::appendChild(wndBase *child) {
  if (children.hasItem(child)) return;
  children.appendItem(child);
}

void wndBase::removeChild(wndBase *child, BOOL kill) {
  if (!children.hasItem(child)) return;
  children.removeItem(child);
  if (kill) delete child;
}

wndBase *wndBase::getChild(HWND wnd) {
  wndBase *wndBase = children.getFirstItem();
  while (wndBase) {
    if (wndBase->getWnd() == wnd) return wndBase;
    wndBase = children.getNextItem(wndBase);
  }
  return NULL;
}

void wndBase::setAlwaysOnTop(BOOL ontop) {
  if (ontop) {
    SetWindowPos(getWnd(), HWND_TOPMOST,   0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
  } else {
    SetWindowPos(getWnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
  }
}


void wndBase::setVisible(BOOL vis) {
  vis = !!vis;
  if (vis == isVisible()) return;
  if (vis) ShowWindow(getWnd(), SW_SHOW);
  else ShowWindow(getWnd(), SW_HIDE);
  onSetVisible(vis);
  invalidate();
}


BOOL wndBase::isVisible() const {
  return !!(getWindowStyle() & WS_VISIBLE);
}

BOOL wndBase::isParentsVisible() const {
  return IsWindowVisible(getWnd());
}


void wndBase::hideChildren(BOOL recursive) {
  wndBase *child = children.getFirstItem();
  while (child) {
    child->hideWindow();
    if (recursive) child->hideChildren(recursive);
    child = children.getNextItem(child);
  }
}

void wndBase::showChildren(BOOL recursive) {
  wndBase *child = children.getFirstItem();
  while (child) {
    child->showWindow();
    if (recursive) child->showChildren(recursive);
    child = children.getNextItem(child);
  }
}


void wndBase::updateNameFromWnd() {
  int len = getWindowTextLength()+1;
  if (len < 2) {
    *this = "";
    return;
  }

  wchar_t *buffer = (wchar_t*) malloc(len*2);
  GetWindowText(getWnd(), buffer, len);
  
  rmxToUtf8 str(buffer);
  if (VSTRCMP(str.getBuffer(), getName("")) != 0) *this = str.getBuffer();

  free(buffer);
}


void wndBase::getWindowText(char *buffer, int length) const {
  GetWindowTextA(getWnd(), buffer, length);
}


void wndBase::onNameChange(const char *newname) {
//  vMessageWnd::onNameChange(newname);
  rmxUtf8ToWide str(*this);
  sendMessage(WM_SETTEXT, NULL, (LPARAM)str.getBuffer());
}


void wndBase::setFont(const char *name, int size) {
  HDC testhdc = GetDC(NULL);
  long lfHeight = -MulDiv(size, GetDeviceCaps(testhdc, LOGPIXELSY), 72);
  ReleaseDC(NULL, testhdc);

  rmxUtf8ToWide str(name);
  setFont( CreateFont(lfHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, str.getBuffer()) );
}

void wndBase::setFont(HFONT font, BOOL redraw) {
  if (hfont) DeleteObject(hfont);
  hfont = font;
  sendMessage(WM_SETFONT, (UINT)hfont, !!redraw);
}

void wndBase::setIcon(HICON hIcon) {
  if (hicon) DestroyIcon(hicon);
  hicon = hIcon;
  sendMessage(WM_SETICON, ICON_SMALL, (LPARAM)hicon);
  sendMessage(WM_SETICON, ICON_BIG,   (LPARAM)hicon);
}

void wndBase::setIcon(WORD resource) {
  HICON hIcon = (HICON)LoadImage(RMXOS::getInstance(), MAKEINTRESOURCE(resource), IMAGE_ICON, 0, 0, LR_SHARED);
  setIcon(hIcon);
}


void wndBase::setId(UINT newid) {
  if (getWnd()) SetWindowLongPtr(getWnd(), GWLP_ID, newid);
  id = newid;
}

VUINT wndBase::getId() const {
  return getLong(GWLP_ID);
}


void wndBase::resize(int w, int h, BOOL redraw) {
  updateSizeRect(w, h, TRUE, redraw);
}

void wndBase::resizeClient(int w, int h, BOOL redraw) {
  resize(getWidth()-getClientWidth()+w, getHeight()-getClientHeight()+h, redraw);
}

void wndBase::move(int x, int y, BOOL redraw) {
  updateMoveRect(x, y, TRUE, redraw);
}

void wndBase::moveresize(int x, int y, int w, int h, BOOL redraw) {
  updateMoveSizeRect(x, y, w, h, TRUE, redraw);
}

int wndBase::getX() const {
  return pos.left;
}

int wndBase::getY() const {
  return pos.top;
}

void wndBase::setX(int x, BOOL redraw) {
  move(x, getY(), redraw);
}

void wndBase::setY(int y, BOOL redraw) {
  move(getX(), y, redraw);
}

void wndBase::setWidth(int w, BOOL redraw) {
  resize(w, getHeight(), redraw);
}

void wndBase::setHeight(int h, BOOL redraw) {
  resize(getWidth(), h, redraw);
}


int wndBase::getWidth() const {
  return pos.right;
}

int wndBase::getHeight() const {
  return pos.bottom;
}

int wndBase::getClientWidth() const {
  RECT rect;
  getClientRect(&rect);
  return rect.right - rect.left;
}

int wndBase::getClientHeight() const {
  RECT rect;
  getClientRect(&rect);
  return rect.bottom - rect.top;
}

int wndBase::getWindowWidth() const {
  RECT rect;
  getWindowRect(&rect);
  return rect.right - rect.left;
}

int wndBase::getWindowHeight() const {
  RECT rect;
  getWindowRect(&rect);
  return rect.bottom - rect.top;
}


void wndBase::updateMoveRect(int x, int y, BOOL update, BOOL redraw) {
  if ( (x == pos.left) & (y == pos.top) ) return;
  pos.left = x;
  pos.top = y;
  if (update) {
    UINT flags = SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER;
    if (!redraw) flags |= SWP_NOREDRAW;
    if (getWnd()) SetWindowPos(getWnd(), NULL, x, y, 0, 0, flags);
  }
  if (isInited()) onMove(x, y);
}

void wndBase::updateSizeRect(int w, int h, BOOL update, BOOL redraw) {
  if ( (w == pos.right) & (h == pos.bottom) ) return;
  pos.right = w;
  pos.bottom = h;
  if (update) {
    UINT flags = SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
    if (!redraw) flags |= SWP_NOREDRAW;
    if (getWnd()) SetWindowPos(getWnd(), NULL, 0, 0, w, h, flags);
  }
  if (isInited()) onResize(getClientWidth(), getClientHeight());
}

void wndBase::updateMoveSizeRect(int x, int y, int w, int h, BOOL update, BOOL redraw) {
  if ( (w == pos.right) && (h == pos.bottom) )   { updateMoveRect(x, y, update, redraw); return; }
  else if ( (x == pos.left)  && (y == pos.top) ) { updateSizeRect(w, h, update, redraw); return; }

  pos.left = x;
  pos.top = y;
  pos.right = w;
  pos.bottom = h;
  if (update) {
    UINT flags = SWP_NOOWNERZORDER | SWP_NOZORDER;
    if (!redraw) flags |= SWP_NOREDRAW;
    if (getWnd()) SetWindowPos(getWnd(), NULL, x, y, w, h, flags);
  }
  if (isInited()) {
    onMove(x, y);
    onResize(getClientWidth(), getClientHeight());
  }
}

void wndBase::updateWindowPosition() {
  if (!getvParent()) {
    getWindowRect(&pos);
    return;
  }
  RECT r1, r2;
  getvParent()->getWindowRect(&r1);
  getWindowRect(&r2);
//  pos.left = r2.left - r1.left;
//  pos.top = r2.top - r1.top;
  pos.right = r2.right - r2.left;
  pos.bottom = r2.bottom - r2.top;
}


void wndBase::fakeResize() {
  if (!isInited()) return;
  RECT r;
  getClientRect(&r);
  onResize(r.right, r.bottom);
}

void wndBase::setBackgroundBrush(HBRUSH brush, BOOL repaint) {
  bgbrush = brush;
  if (repaint) if (getWnd()) invalidate(NULL, TRUE);
}

HBRUSH wndBase::getBackgroundBrush() const {
  if ( bgbrush == ((HBRUSH)-1) ) {
    if (getvParent()) return getvParent()->getBackgroundBrush();
    return NULL;
  }
  return bgbrush;
}


BOOL wndBase::paintevent() {
  if (!customPaint()) return FALSE;
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(getWnd(), &ps);
  onPaint(hdc, &ps);
  EndPaint(getWnd(), &ps);
  return TRUE;
}


void wndBase::invalidate(RECT *rect, BOOL bgErase) {
  if (!getWnd()) return;
  InvalidateRect(getWnd(), rect, !!bgErase);
}




void wndBase::closeWnd(BOOL passive) {
  if (passive) postMessage(WM_CLOSE);
  else sendMessage(WM_CLOSE);
}


void wndBase::setAlpha(int alpha) {
  if (!RMXOS::isWin50()) return;

//  UINT style = getWindowStyle();
//  if (!(style & WS_EX_LAYERED)) setWindowStyle(style & WS_EX_LAYERED);
//  SetLayeredWindowAttributes(getWnd(), 0, alpha, LWA_ALPHA);
}


VUINT wndBase::wndProc(UINT msg, VUINT param1, VUINT param2) {
  switch (msg) {
    case WM_PAINT: if (paintevent()) return 0; break;
    case WM_ERASEBKGND: if (eraseBackground((HDC)param1)) return 1; break;
//    case WM_SIZE: onResize(LOWORD(lParam), HIWORD(lParam)); break;
//    case WM_MOVE:  updateMoveRect(LOWORD(lParam), HIWORD(lParam)); break;
//    case WM_POSCHANGING
//    case WM_COMMAND: /*return*/ onCommand(LOWORD(param1), HIWORD(param1), param2); break;
    case WM_SETFOCUS:  onSetFocus();  return 0;
    case WM_KILLFOCUS: onLoseFocus(); return 0;

    case WM_TIMER: onTimer((DWORD)param1, RMXOS::getTick());  break;

    case WM_WINDOWPOSCHANGED: {
      if (!getParentWnd()) {
        WINDOWPOS *pos = (WINDOWPOS*)param2;
        if (pos) {
          if (!(pos->flags & SWP_NOMOVE)) updateMoveRect(pos->x,  pos->y,  FALSE);
          if (!(pos->flags & SWP_NOSIZE)) updateSizeRect(pos->cx, pos->cy, FALSE);
        }
      }
    } break;

    case WM_NOTIFY: {
      NMHDR *n = (NMHDR*)param2;
      if (n->code == TTN_GETDISPINFO) {
        LPNMTTDISPINFO info = (LPNMTTDISPINFO) param2;
        SendMessage(n->hwndFrom, TTM_SETMAXTIPWIDTH, 0, 500);
        if (tooltip) {
          rmxUtf8ToWide str(tooltip);
          if (str.getBuffer()) {
            WSTRCPY(info->szText, str.getBuffer(), sizeof(info->szText)/2);
          }
        } else {
          WSTRCPY(info->szText, L"", 1);
        }
        return 0;
      }
    } break;

    case WM_CLOSE:
      if (flags.deleting) onClose();
      else if (onClose()) {
        delete this;
        return 0;
      }
  }

  whkInfo hookinfo;
  hookinfo.wnd = this;
  hookinfo.msg = msg;
  hookinfo.param1 = param1;
  hookinfo.param2 = param2;
  hookinfo.ret = 0;

  whkBase *hook = hooks.getFirstItem();
  while (hook) {
    if (hook->wndHookProc(&hookinfo)) return hookinfo.ret;
    hook = hooks.getNextItem(hook);
  }


  return vMessageWnd::wndProc(msg, param1, param2);
}



void wndBase::setTooltip(const char *tip) {
  tooltip.setName(tip);

  TOOLINFO ti;

  if (htool == NULL) {
    htool = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, getWnd(), NULL, RMXOS::getInstance(), NULL);

    if (!htool) {
      MessageBox(0, L"Error Creating Tooltip", L"", 0);
      return;
    }

    SetWindowPos(htool, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
  }

  RECT rect;
  getClientRect(&rect);

  // INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE
  ZeroMemory(&ti, sizeof(ti));
  ti.cbSize   = sizeof(TOOLINFO);
  ti.uFlags   = TTF_SUBCLASS;
  ti.hwnd     = getWnd();
  ti.hinst    = RMXOS::getInstance();
  ti.lpszText = LPSTR_TEXTCALLBACK;

  // ToolTip control will cover the whole window
  ti.rect.left   = rect.left;    
  ti.rect.top    = rect.top;
  ti.rect.right  = 65535;
  ti.rect.bottom = 65535;

  // SEND AN ADDTOOL MESSAGE TO THE TOOLTIP CONTROL WINDOW 
  if (!SendMessage(htool, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti)) {
    MessageBox(0, L"Error Assigning Tooltip", L"", 0);
    return;
  }
}


#endif
