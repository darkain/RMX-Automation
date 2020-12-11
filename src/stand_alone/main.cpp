/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../../sdk/plugBase.h"

#include "../../sdk/Win32/Win32.h"

#include "../core/rmxMasterPlugin.h"
#include "../core/sqlite/sqlite.h"


#include "main.h"
#include "resource.h"

#include <shellapi.h>


#ifndef RMX_STANDALONE
#error "RMX predefine.h configuration conflicts with RMX_STANDALONE"
#endif


#define APPNAME    "RMX Automation 2"
#define APPVERSION APPNAME " - " _APP_VERSION


int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
  RMXOS::setInstance(hInst);

  {
    HWND hRmx = FindWindowA(APPNAME, NULL);
    if (hRmx) return (int) SendMessage(hRmx, WM_USER+5, 0, 0);
  }

  ShowWindow(0, 0);  //clear cached ShowWindow krap that windows does

  saPlugins *plugins = new saPlugins;
  
  SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);


  MSG msg;
  while (plg)  {
    ZeroMemory(&msg, sizeof(MSG));
#ifdef _DEBUG
    if (GetMessage(&msg, NULL, 0, 0) < 0) MessageBox(0, L"GetMessage() Error", L"ERROR!!", 0);
#else
    GetMessage(&msg, NULL, 0, 0);
#endif
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return (int) msg.lParam;
}



//--------------------------------------------------------------------------------------------------------------------------


class saTest : public funcSingle {
public:
	saTest(cfgRoot *parent) : funcSingle("TEST FUNCTION", "test", parent) {}
	virtual ~saTest() {}

protected:
	virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
//		HPARAM param;
/*
		param = source->list_insertParam(output, "param");
		source->param_setValue(param, "text");
		
		param = source->list_insertParam(output, "integer");
		source->param_setValueInt(param, 123456);
		
		param = source->list_insertParam(output, "long");
		source->param_setValueLong(param, 0x1234567890);
		
		param = source->list_insertParam(output, "number");
		source->param_setValueNumber(param, 0.2f);
		
		param = source->list_insertParam(output, "string");
		source->param_setValueString(param, "this is a string");
		
		param = source->list_insertParam(output, "guid");
		source->param_setValueGuid(param, _INVALID_GUID);
		
		param = source->list_insertParam(output, "fourcc");
		source->param_setValueFourCC(param, 'ABCD');*/
	}
};


//--------------------------------------------------------------------------------------------------------------------------


saPlugins::saPlugins() {
#ifdef _RMX_SQLITE_
	sqlite->setSetting("_rmx_title", APPNAME);
	sqlite->setSetting("_rmx_full",  APPVERSION);
#endif //_RMX_SQLITE_

	{
		char path[MAX_PATH] = "";
		VPRINTF(path, sizeof(path), "%s\\%s", RMXOS::getPath(), RMXOS::getName());
		setMasterPlugin(new rmxMasterPlugin(path));
	}

	onInit();
}


saPlugins::~saPlugins() {
	PostQuitMessage(0);
}


void saPlugins::initMasterLists(rmxMasterPlugin *master) {
	//do rest of init stuff
	vPluginLoader::initMasterLists(master);

	//create thingy to launch apps
	new saLaunch(master);

#ifdef _DEBUG
	//create testing object
	new saTest(master);
#endif 

	//create thingy to exit rmx
	new saExitFunction(master);
}



//--------------------------------------------------------------------------------------------------------------------------


saLaunch::saLaunch(cfgRoot *parent) : funcSingle("Launch", "shell", parent) {
  new funcParam("Application (path to .exe)", "f_path", "string", this);
  new funcParam("Command Line Arguments",     "f_args", "string", this);
}


saLaunch::~saLaunch() {
  if (!plg) return;
  vLocalPlugin *plugin = plg->getMasterPlugin();
  if (!plugin) return;
}


void saLaunch::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  vPlugin *plugin = plg->getPlugin(source);
  if (!plugin) return;

  const char *path = plugin->list_getParam(input, "f_path");
  if (!path) return;

  const char *args = plugin->list_getParam(input, "f_args");

  rmxThread *thread = new saLaunchThread(path, args);
  thread->Begin(THREAD_PRIORITY_NORMAL);
}


int saLaunchThread::threadProc() {
  rmxUtf8ToWide filepath(path);
  rmxUtf8ToWide fileargs(args);

  ShellExecute(NULL, L"open", filepath.getBuffer(), fileargs.getBuffer(), L"", SW_SHOW);
  return rmxThread::threadProc();
}
