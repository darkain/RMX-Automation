/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _COMMON_H_
#define _COMMON_H_


#include "../../sdk/Win32/Win32.h"
#include "../rmx_win32/rmx_win32.h"


BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
BOOL WINAPI _DllMainCRTStartup(HANDLE hInst, ULONG ul_reason_for_call, LPVOID lpReserved);


#ifdef _RMX_MEDIA_REMOTE_
#define APPNAME    "RMX Remote Server"
#else //_RMX_MEDIA_REMOTE_
#define APPNAME    "RMX Automation - Media Edition"
#endif //_RMX_MEDIA_REMOTE_

#define APPVERSION APPNAME " - " _APP_VERSION


void rmx_config();
void rmx_quit();
void rmx_init(int tick);


extern vRmxWin32 *rmx;



#endif //_COMMON_H_
