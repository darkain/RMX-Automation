/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../core/LocalPlugin.h"
#include "engine.h"
#include "engWnd.h"



#ifndef RMX_GAMINGEDITION
#error "RMX predefine.h configuration conflicts with RMX_GAMINGEDITION"
#endif


#define APPNAME    "RMX Engine"
#define APPVERSION APPNAME " - " _APP_VERSION


vEngine *ENGINE = NULL;


/////////////////////////////////////////////////////////////////////////////


int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
  RMXOS::setInstance(hInst);


  ShowWindow(0, 0);  //clear cached ShowWindow krap that windows does

  vEngine *engine = new vEngine();
  engine->init();
//  engine->loadSettings();
  
  SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);

  
  MSG msg;
  while (ENGINE)  {
    ZeroMemory(&msg, sizeof(MSG));
#ifdef _DEBUG
    if (GetMessage(&msg, NULL, 0, 0) < 0) MessageBox(0, L"GetMessage() Error", L"ERROR!!", 0);
#else
    GetMessage(&msg, NULL, 0, 0);
#endif
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.lParam;
}



/////////////////////////////////////////////////////////////////////////////



vEngine::vEngine() {
  ENGINE = this;

  setWindowName(APPNAME);
  setClassName(APPNAME);

  wnd = NULL;
}


vEngine::~vEngine() {
  vdelnull(wnd);
  PostQuitMessage(0);

  ENGINE = NULL;
}


void vEngine::initMasterPlugin(vLocalPlugin *master) {
  cfgBase *ident = master->getID();  //TODO: ident is now deprecated, replaced by sql "settings" table
  ident->insertParamNoSave(   "host",    "Engine");
  ident->insertParamNoSave(   "title",    APPVERSION  );
  ident->insertParamNoSave(   "appname",  APPNAME     );
  ident->insertParamNoSave(   "version", _APP_VERSION );
  ident->insertParamIntNoSave("build",   _SDK_BUILD   );
  
  vRmxWin32::initMasterPlugin(master);
}


BOOL vEngine::onRmxPostInit() {
  BOOL ret = vRmxWin32::onRmxPostInit();

  wnd = new engWnd(APPNAME, getMasterPlugin());
  wnd->init();
  wnd->showWindow();

  return ret;
}
