/******************************  RMX SDK  ******************************\
*  Copyright (c) 2011-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../predefine.h"



#ifdef _RMX_WIN32_

#if   defined _RMX_WIN64_
#error "_RMX_WIN32_" and "_RMX_WIN64_" may not be defined at the same time!
#elif defined _RMX_LINUX_
#error "_RMX_WIN32_" and "_RMX_LINUX_" may not be defined at the same time!
#endif

#undef WINVER
#undef _WIN32_WINNT
#undef _WIN32_IE

#define WINVER 0x0600
#define _WIN32_WINNT 0x0600
#define _WIN32_IE 0x0800
#define WIN32_LEAN_AND_MEAN
#define UNICODE
#include <windows.h>

#endif //_RMX_WIN32_


////////////////////////////////////////////////////////////////////////////////


#if defined _RMX_LINUX_

#include <pthread.h>

#endif //_RMX_LINUX_
