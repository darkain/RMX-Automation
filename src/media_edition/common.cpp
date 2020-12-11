/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../core/LocalPlugin.h"
#include "common.h"


#ifndef RMX_MEDIAEDITION
#error "RMX predefine.h configuration conflicts with RMX_MEDIAEDITION"
#endif



#ifdef RMX_WMP //ATL is only required for Windows Media Player support

//ATL shit
#define _ATL_APARTMENT_THREADED
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <initguid.h>
#include "wmp/wmpMain.h"

//more ATL shit
BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_wmpRmx, CwmpRmx)
END_OBJECT_MAP()
//end of ATL shit

#else //RMX_WMP

STDAPI DllRegisterServer(void) { return NULL; }
STDAPI DllUnregisterServer(void) { return NULL; }

#endif //RMX_WMP


vRmxWin32 *rmx = NULL;


BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
  switch (ul_reason_for_call) {

    case DLL_PROCESS_ATTACH:
      RMXOS::setInstance((HINSTANCE)hModule);

#ifdef RMX_WMP //More stupid ATL krap
      _Module.Init(ObjectMap, (HINSTANCE)hModule);
#endif //RMX_WMP

      DisableThreadLibraryCalls((HMODULE)hModule);
      break;


#ifdef RMX_WMP //More stupid ATL krap
    case DLL_PROCESS_DETACH:
      _Module.Term();
      break;
#endif //RMX_WMP
  }

  return TRUE;
}



BOOL WINAPI _DllMainCRTStartup(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved) {
  return TRUE;
}


STDAPI DllCanUnloadNow(void) {
#ifdef RMX_WMP
	return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
#else 
	return S_OK;
#endif //RMX_WMP
}


STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
#ifdef RMX_WMP
	return _Module.GetClassObject(rclsid, riid, ppv);
#else 
	return NULL;
#endif //RMX_WMP
}


void rmx_init(int tick) {
  if (!rmx) return;

  rmx->init();
//  rmx->loadSettings();
}


void rmx_quit() {
  if (!rmx) return;
  rmx->destroy();
  rmx = NULL;
}


void rmx_config() {
  if (!rmx) return;

#ifdef _RMX_MEDIA_REMOTE_
  MessageBoxA(0, "Please access \"RMX Remote\" on your mobile phone", rmx->getMasterPlugin()->getID()->getParamSafe("appname"), 0);
#else //_RMX_MEDIA_REMOTE_
  plg->getMasterPlugin()->onMessage((HPLUGIN)plg->getMasterHandle(), GENERIC_OPEN, 0, 0);
#endif //_RMX_MEDIA_REMOTE_
}
