/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RMX_BASE_H_
#define _RMX_BASE_H_


#include "../predefine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rmxTypes.h"
#include "rmxUnicode.h"


#include "handles/hplugin.h"
#include "handles/hconsole.h"
#include "handles/hfunction.h"
#include "handles/hparamlist.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifndef CHAT
#define CHAT(from, to, text) message(__FILE__ "(" WARNING_TOSTR1(__LINE__) ") : <" from "> " to ": " text)
#define SELF(from, text) message(__FILE__ "(" WARNING_TOSTR1(__LINE__) ") : * " from"/#rmx " text)
#endif //CHAT


#define VSTRCMP  strcmp
#define VSTRLEN  strlen
#define WSTRLEN  wcslen
BOOL VFEXISTS(const char *path);
VINT VFSIZE(  const char *path);
int RAND();


#if defined (_MSC_VER) && (_MSC_VER >= 1400)
#define VPRINTF sprintf_s
#define VSCANF  sscanf_s
#define VSTRCMPI _strcmpi
#define VSTRCPY(dst, src, len) strncpy_s(dst, len, src, VSTRLEN(src))
#define VSTRCAT(dst, src, len) strncat_s(dst, len, src, VSTRLEN(src))
#define WPRINTF swprintf_s
#define WSTRCPY(dst, src, len) wcsncpy_s(dst, len, src, WSTRLEN(src))
#define WSTRCAT(dst, src, len) wcsrncat_s(dst, len, src, WSTRLEN(src))
#define VFOPEN(file, filename, mode) fopen_s(file, filename, mode)
#else
#define VPRINTF _snprintf
#define VSCANF   sscanf
#define VSTRCPY  strncpy
#define VSTRCMPI strcmpi
#define VSTRCAT  strncat
#define WPRINTF _snwprintf
#define WSTRCMP  wstrcmp
#define WSTRCMPI wstrcmpi
#define WSTRCPY  wcsncpy
#define WSTRCAT  wstrncat
int VFOPEN(FILE **file, const char *path, const char *open);
#endif


#define vdelnull(x) { delete x; x=NULL; }
#define vrelease(x) { if(x) x->Release(); x=NULL; } 
#define vrestore(x) { if(x) x->Restore(); }
#define vclose(x)   { if (x!=INVALID_HANDLE_VALUE) CloseHandle(x); x=INVALID_HANDLE_VALUE; }


#if defined _WIN32  ||  defined _WIN64
#  define RMXOS vWin32
#else
#  error PORT ME
#endif


#ifdef _CPPRTTI
#define RMX_CAST dynamic_cast
#else
#define RMX_CAST static_cast
#endif


#ifdef __cplusplus
}  //extern "C"
#endif


#endif //_RMX_BASE_H_
