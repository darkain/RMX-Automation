/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


//TODO: buttons dont seem to be expanding correctly (sometimes?)


#pragma comment(lib, "comctl32.lib")


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/cfgAddChild.h"
#include "Toolbar.h"
#include <commctrl.h>


static Toolbar *TOOLBAR = new Toolbar();


Toolbar::Toolbar() : plugBase("Toolbar Controller") {
  tree = NULL;
}


void Toolbar::onInit() {
  tree = new vToolbarRoot(getName(), this);

  new tcfgAddChildExpand<ToolbarButton>("Add Toolbar Button", "Toolbar Button", tree);
  
  wnd = new ToolbarParentWnd(getName());
  wnd->setBackgroundBrush((HBRUSH)GetSysColorBrush(COLOR_ACTIVECAPTION));
  wnd->init();
  wnd->showWindow();

  cfgBase *pos = new cfgWndPos<cfgNull>("Position", tree, wnd);
  new vToolbarOnTop(pos, wnd);
  new cfgLine(tree);
}


void Toolbar::onQuit() {
  vdelnull(wnd);
  vdelnull(tree);
}


//---------------------------------------------------------------------------------------------------------------------


void vToolbarRoot::onLoadSettings() {
  ToolbarTreeParent::onLoadSettings();
  TOOLBAR->getWindow()->fakeResize();
}


void vToolbarRoot::onInsertChild(cfgBase *child, const cfgBase *insert) {
  ToolbarTreeParent::onInsertChild(child, insert);
  TOOLBAR->getWindow()->fakeResize();
}


void vToolbarRoot::onRemoveChild(const cfgBase *child) {
  ToolbarTreeParent::onRemoveChild(child);
  TOOLBAR->getWindow()->fakeResize();
}


void vToolbarRoot::onPreMoveChild(const cfgBase *child, const cfgBase *insert) {
  ToolbarTreeParent::onPreMoveChild(child, insert);

  int index1 = getUniqueIndex(child);
  int index2 = getUniqueIndex(insert);

  if (index2 == -1)         index2 = getUniqueCount();
  if (index2 - index1 == 2) index2 = getUniqueIndex(getNextUnique(child));

  wndBase *wnd = TOOLBAR->getWindow()->getToolbar();
  wnd->sendMessage(TB_MOVEBUTTON, index1, index2);
}



//---------------------------------------------------------------------------------------------------------------------



ToolbarParentWnd::ToolbarParentWnd(const char *name) : wndBase(name) {
  setWindowStyle(WS_POPUP | WS_OVERLAPPED);
  setWindowStyleEx(WS_EX_TOOLWINDOW);
  setName(name);

  toolbar = NULL;
  rebar = NULL;
}


ToolbarParentWnd::~ToolbarParentWnd() {
}


void ToolbarParentWnd::onInit() {
  wndBase::onInit();

  ShowWindow(getWnd(), SW_HIDE);
  SetWindowPos(getWnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);  //TODO: testing this

  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
  InitCommonControlsEx(&icex);

  rebar = new wndSubclass(REBARCLASSNAME, this);
  rebar->setWindowStyleEx(WS_EX_TOOLWINDOW);  //TODO:  see if this is even needed
  rebar->setWindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | RBS_VARHEIGHT | CCS_NODIVIDER | CCS_NOPARENTALIGN);
  rebar->init();
  rebar->showWindow();
  insertHookWnd(rebar);

  REBARINFO info;
  ZeroMemory(&info, sizeof(REBARINFO));
  info.cbSize = sizeof(REBARINFO);
  info.fMask = 0;
  info.himl = (HIMAGELIST)NULL;
  rebar->sendMessage(RB_SETBARINFO, 0, (UINT)&info);

  toolbar = new ToolbarWnd(this);
  toolbar->init();
  toolbar->showWindow();

  REBARBANDINFO band;
  ZeroMemory(&band, sizeof(REBARBANDINFO));
  band.cbSize = sizeof(REBARBANDINFO);
  band.fMask = RBBIM_STYLE | RBBIM_CHILD | RBBIM_CHILDSIZE;
  band.fStyle = RBBS_GRIPPERALWAYS;

  band.hwndChild = toolbar->getWnd();
  band.cxMinChild = 0;
  band.cyMinChild = 22;
  band.cx = 250;
  rebar->sendMessage(RB_INSERTBAND, (UINT)-1, (UINT)&band);
}


void ToolbarParentWnd::onResize(int width, int height) {
  if (TOOLBAR->getTree()->getUniqueCount() < 1) {
    hideWindow();
    return;
  }

  if (!wndBase::isVisible()) showWindow();

  RECT rect = {0,0,0,0};
  SIZE size;
  RECT rect2 = {0,0,0,0};
  toolbar->sendMessage(TB_GETMAXSIZE, 0, (UINT)&size);
  rebar->sendMessage(RB_GETBANDBORDERS, 0, (UINT)&rect2);

  rect.right  = size.cx;
  rect.bottom = size.cy;
  AdjustWindowRectEx(&rect, getWindowStyle(), FALSE, getWindowStyleEx());
  width  = rect.right - rect.left + rect2.left + 6;
  height = rect.bottom - rect.top + 3;

//  if (width==nwidth && height==nheight) return;
  resize(width, height);

  wndBase::onResize(width, height);
  if (rebar) MoveWindow(rebar->getWnd(), 0, 0, width, height, TRUE);
}


VUINT ToolbarParentWnd::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == WM_GETMINMAXINFO) {
    MINMAXINFO *info = (MINMAXINFO*)param2;
    RECT rect = {0,0,0,0};
    SIZE size;
    RECT rect2 = {0,0,0,0};

    toolbar->sendMessage(TB_GETMAXSIZE, 0, (UINT)&size);
    rebar->sendMessage(RB_GETBANDBORDERS, 0, (UINT)&rect2);

    rect.right  = size.cx;
    rect.bottom = size.cy;
    AdjustWindowRectEx(&rect, getWindowStyle(), FALSE, getWindowStyleEx());
    int width  = rect.right - rect.left + rect2.left + 6;
    int height = rect.bottom - rect.top + 3;
    
    info->ptMaxSize.x      = width;
    info->ptMaxPosition.x  = width;
    info->ptMinTrackSize.x = width;
    info->ptMaxTrackSize.x = width;

    info->ptMaxSize.y      = height;
    info->ptMaxPosition.y  = height;
    info->ptMinTrackSize.y = height;
    info->ptMaxTrackSize.y = height;

    return 0;
  }

  return wndBase::wndProc(msg, param1, param2);
}


BOOL ToolbarParentWnd::wndHookProc(whkInfo *info) {
  if (info->msg == WM_LBUTTONDOWN) {
    sendMessage(WM_SYSCOMMAND, 61458);
    info->ret = 0;
    return TRUE;
  }

//  if (info->msg == WM_LBUTTONUP) {
//    ReleaseCapture();
//    info->ret = 0;
//    return TRUE;
//  }

  return whkBase::wndHookProc(info);
}


//---------------------------------------------------------------------------------------


ToolbarWnd::ToolbarWnd(wndBase *parent) : wndBase(TOOLBARCLASSNAME, parent) {
  whkCommand::insertHookWnd(parent);
  appendWindowStyle(TBSTYLE_TOOLTIPS | TBSTYLE_FLAT | TBSTYLE_LIST | CCS_NORESIZE | CCS_NOPARENTALIGN);
}


ToolbarWnd::~ToolbarWnd() {
  TOOLBAR->getTree()->removeAllUniques();
}



void ToolbarWnd::onInit() {
  wndBase::onInit();

  sendMessage(TB_SETBITMAPSIZE, 0, 0);
  sendMessage(TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
}


BOOL ToolbarWnd::onCommand(whkInfo *info) {
  cfgBase *child = TOOLBAR->getTree()->getFirstChild();
  while (child) {
    if (child->getParamInt("tbid") == LOWORD(info->param1)) {
      evntSingle *event = RMX_CAST<evntSingle*>(child);
      if (event) event->activate();
    }
    child = TOOLBAR->getTree()->getNextChild(child);
  }
  return FALSE;
}


//---------------------------------------------------------------------------------------


int ToolbarButton::btnCount = 0;


ToolbarButton::ToolbarButton(const char *name, cfgBase *parent, HPARAMLIST paramlist)
 : evntSingle(name, parent, NULL, paramlist) {

  (insertParamIntNoSave("tbid", ++btnCount))->setReadOnly(TRUE);

  TBBUTTON buttons[1];
  ZeroMemory(&buttons, sizeof(buttons));
  buttons[0].idCommand = getParamInt("tbid");
  buttons[0].fsState = TBSTATE_ENABLED; 
  buttons[0].fsStyle = TBSTYLE_BUTTON | BTNS_AUTOSIZE;
  buttons[0].iString = (VINT)L"Button";
  TOOLBAR->getWindow()->getToolbar()->sendMessage(TB_ADDBUTTONS, 1, (UINT)&buttons);

  edit = new ToolbarText(this);
  edit->setText("Button");

  new tcfgMoveUniqueUp<  ToolbarTreeParent>("Move Up",   "Move Up",   this);
  new tcfgMoveUniqueDown<ToolbarTreeParent>("Move Down", "Move Down", this);
  new cfgRemoveParent("Remove Toolbar Button", this);
}


ToolbarButton::~ToolbarButton() {
  TOOLBAR->getWindow()->getToolbar()->sendMessage(TB_DELETEBUTTON, (UINT)TOOLBAR->getTree()->getUniqueIndex(this));
}


void ToolbarButton::setToolbarButtonText(const char *text) {
  rmxUtf8ToWide name(text);

  TBBUTTONINFO info;
  ZeroMemory(&info, sizeof(info));
  info.cbSize  = sizeof(info);
  info.dwMask  = TBIF_TEXT;
  info.pszText = (LPWSTR)name.getBuffer();
  TOOLBAR->getWindow()->getToolbar()->sendMessage(TB_SETBUTTONINFO, getParamInt("tbid"), (UINT)&info);
  TOOLBAR->getWindow()->fakeResize();
}
