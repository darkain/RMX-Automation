/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/
#if 0


#include "../../../sdk/rmxBase.h"
#include "../../../sdk/Win32/Win32.h"
#include <cderr.h>
#include <shellapi.h>

#include "ConfigPaneWnd.h"
#include "ConfigPaneItems.h"


//---------------------------------------------------------------------------------------------------------


vConfigPaneLine::vConfigPaneLine(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert)
 : vForestLine(parent, insert), vConfigPaneItem(panewnd, cb_plugin, cb_list) {
  setForestItem(this);
}


//---------------------------------------------------------------------------------------------------------


vConfigPaneNull::vConfigPaneNull(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert)
 : wndForestItem(parent, insert), vConfigPaneItem(panewnd, cb_plugin, cb_list) {
  setForestItem(this);
  updateAlign();
}


//void vConfigPaneNull::cb_onSetParam(HPARAM param) {
void vConfigPaneNull::cb_cfgMessage(cbMessage message, HPARAM param, HPARAM insert) {
  if (message == CB_PARAM_UPDATED) {
    const char *name = getRemotePlugin()->param_getName(param);
    if (name  &&  VSTRCMP(name, "align")==0) updateAlign();
  }

  vConfigPaneItem::cb_cfgMessage(message, param, insert);
//  vConfigPaneItem::cb_onSetParam(param);
}


void vConfigPaneNull::updateAlign() {
  const char *value = getParamSafe("align");
  if (VSTRCMP(value, "center") == 0) {
    setTextHAlign(TEXTHALIGN_CENTER);
  } else if (VSTRCMP(value, "right") == 0) {
    setTextHAlign(TEXTHALIGN_RIGHT);
  } else {
    setTextHAlign(TEXTHALIGN_LEFT);
  }
}


//---------------------------------------------------------------------------------------------------------


vConfigPaneLink::vConfigPaneLink(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert)
 : vConfigPaneNull(panewnd, cb_plugin, cb_list, parent, insert) {
  setCursor(LoadCursor(NULL, IDC_HAND));
  setNormalColor(GetSysColor(COLOR_HOTLIGHT));
}


void vConfigPaneLink::onLeftDown(int x, int y) {
  vConfigPaneNull::onLeftDown(x, y);
  rmxUtf8ToWide str(getUserData());
  ShellExecute(NULL, L"open", str.getBuffer(), NULL, L"c:\\", SW_SHOW);
}


//---------------------------------------------------------------------------------------------------------


vConfigPaneButton::vConfigPaneButton(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert)
 : vForestButton(parent, insert), vConfigPaneItem(panewnd, cb_plugin, cb_list) {
  setForestItem(this);
  setButtonText(getParam("text"));
  setDescription(getParam("desc"));
}


//TODO: FIX THIS SHIT!!
void vConfigPaneButton::cb_onSetParam(HPARAM param) {
//  vConfigPaneItem::cb_onSetParam(param);
  HPARAM hparam = (HPARAM)param;
  const char *name = getRemotePlugin()->param_getName(hparam);
  const char *value = getRemotePlugin()->param_getValue(hparam);
  if (VSTRCMP(name, "text") == 0) {
    setButtonText(value);
  } else if (VSTRCMP(name, "desc") == 0) {
    setDescription(value);
  }
}


//---------------------------------------------------------------------------------------------------------


vConfigPaneBool::vConfigPaneBool(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert)
 : vForestBool(parent, insert), vConfigPaneItem(panewnd, cb_plugin, cb_list) {
  setForestItem(this);
}

void vConfigPaneBool::onInit() {
  vForestBool::onInit();
  vConfigPaneItem::onInit();

  //add some sort of lock here so it doesnt reset the values within the list
  setChecked( getUserDataInt() );
}


//TODO: FIX THIS SHIT!!
void vConfigPaneBool::cb_onSetUserData(HPARAM userdata) {
//  vConfigPaneItem::cb_onSetUserData(userdata);
  setChecked( !!getUserDataInt() );
}


//---------------------------------------------------------------------------------------------------------


vConfigPaneSlider::vConfigPaneSlider(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert)
 : vForestSlider(parent, insert), vConfigPaneItem(panewnd, cb_plugin, cb_list) {
  setForestItem(this);
}


void vConfigPaneSlider::onInit() {
  vForestSlider::onInit();
  vConfigPaneItem::onInit();
  setTrackWndMinMan( getParamInt("min"), getParamInt("max") );
  setTrackWndPos( getUserDataInt() );
}


void vConfigPaneSlider::onTrackWnd(int pos) {
  vForestSlider::onTrackWnd(pos);
  setUserDataInt(getTrackWndPos());
}


//TODO: FIX THIS SHIT!!
void vConfigPaneSlider::cb_onSetParam(HPARAM param) {
//  vConfigPaneItem::cb_onSetParam(param);
  HPARAM hparam = (HPARAM)param;
  const char *name = getRemotePlugin()->param_getName(hparam);
  if      (VSTRCMP(name, "min") == 0) setTrackWndMin( getRemotePlugin()->param_getValueInt(hparam) );
  else if (VSTRCMP(name, "max") == 0) setTrackWndMax( getRemotePlugin()->param_getValueInt(hparam) );
}


//TODO: FIX THIS SHIT!!
void vConfigPaneSlider::cb_onSetUserData(HPARAM userdata) {
//  vConfigPaneItem::cb_onSetUserData(userdata);
  setTrackWndPos( getUserDataInt() );
}


//---------------------------------------------------------------------------------------------------------


vConfigPaneEdit::vConfigPaneEdit(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert)
 : vForestEdit(parent, insert), vConfigPaneItem(panewnd, cb_plugin, cb_list) {
  setForestItem(this);
  setEditText( getUserData() );
}



//---------------------------------------------------------------------------------------------------------


vConfigPagePathPicker::vConfigPagePathPicker(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert)
 : wndForestItem(parent, insert), vConfigPaneItem(panewnd, cb_plugin, cb_list) {
  setForestItem(this);

  nullwnd = new vNullColorWnd("NULL", this);
  nullwnd->setBackgroundBrush((HBRUSH)GetSysColorBrush(COLOR_WINDOW));
  nullwnd->appendWindowStyle(WS_BORDER);

  button = new wndSubclass("Button", nullwnd);
  button->setBackgroundBrush((HBRUSH)GetStockObject(HOLLOW_BRUSH));
  button->setName("Browse");
  button->setId(10);

  edit = new wndEdit(nullwnd);
  edit->appendWindowStyle(ES_RIGHT|ES_AUTOHSCROLL);
  edit->setName(getUserData());

  whkMouse::insertHookWnd(nullwnd);
  whkMouse::insertHookWnd(button);
  whkMouse::insertHookWnd(edit);
  whkCommand::insertHookWnd(nullwnd);
}


void vConfigPagePathPicker::onInit() {
  wndForestItem::onInit();
  vConfigPaneItem::onInit();
  nullwnd->init();
  button->init();
  edit->init();
  button->showWindow();
  edit->showWindow();
  nullwnd->showWindow();
}


void vConfigPagePathPicker::onResize(int width, int height) {
  wndForestItem::onResize(width, height);
  
  int w = width >> 1;
  nullwnd->moveresize(w, 0, width-w, getCollapseHeight(), FALSE);
  
  RECT r;
  nullwnd->getClientRect(&r);

  edit->resize(r.right-75, r.bottom, FALSE);
  button->moveresize(r.right-75, 0, 75, r.bottom, FALSE);
}


BOOL vConfigPagePathPicker::onCommand(whkInfo *info) {
  if (info) if (button) if (LOWORD(info->param1) == button->getId()) {
    wchar_t filename[MAX_PATH] = L"";
    rmxUtf8ToWide str(getUserDataSafe());
    WSTRCPY(filename, str.getBuffer(), sizeof(filename)/2);


    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    if (RMXOS::isWin50()) {
      ofn.lStructSize = sizeof(ofn);
    } else {
      ofn.lStructSize = OPENFILENAME_SIZE_VERSION_400;
    }

    ofn.hwndOwner   = getWnd();
    ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
    ofn.lpstrFile   = filename;
    ofn.nMaxFile    = MAX_PATH;
    ofn.Flags       = OFN_EXPLORER | OFN_PATHMUSTEXIST  | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"txt";

    if(GetOpenFileName(&ofn))  {
      rmxToUtf8 out(filename);
      setUserData(out.getBuffer());
#ifdef _DEBUG
//    } else {
//      IntBox(CommDlgExtendedError());
#endif
    }
    return TRUE;
  }
  return wndForestItem::onCommand(info);
}


//TODO: FIX THIS SHIT!!
void vConfigPagePathPicker::cb_onSetUserData(HPARAM userdata) {
//  vConfigPaneItem::cb_onSetUserData(userdata);
  edit->setName(getUserDataSafe());
}

#endif
