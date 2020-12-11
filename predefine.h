/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


/*
* This file contains all of the basic configuration information used
* by ALL RMX related projects.  This includes things such as turning
* on/off DirectX support, Lua scripting, and SQLite storage engine.
*/


#ifndef _RMX_PREDEFINES_
#define _RMX_PREDEFINES_


#include "rmx_version.h"


#define _RMX_BETA_

#define _RMX_WIN32_
//#define _RMX_WIN64_
//#define _RMX_LINUX_

/* ONLY ONE OF THESE SHOULD BE ACTIVE AT A TIME */
//#define RMX_MEDIAEDITION
#define RMX_STANDALONE
//#define RMX_GAMINGEDITION
//#define RMX_PLUGIN
//#define _RMX_MEDIA_REMOTE_



#ifdef _DEBUG
#  define _RMX_DEBUG_
#else
//#  define _RMX_DEBUG_
#endif

#define RMX_PLUGINLOADER
#define _RMX_CORE_CONFIG_
#define _RMX_CORE_CONSOLE_
#define _RMX_CORE_TRAY_


//REMOVE THIS IF BUILDING FOR NON-WINDOWS SYSTEMS, SUCH AS LINUX OR ARM
#define _RMX_WINDOWS_


//#define _RMX_LUA_
#define _RMX_SQLITE_
//#define _RMX_DIRECTX_
//#define _RMX_XML_SETTINGS_


#ifdef _RMX_DEBUG_
#  define DEBUGMSG(name, msg) CONSOLE_MAIN->printDebug(name, msg)
#  define PROFILE()      { char PROF_STR[256]=""; VPRINTF(PROF_STR, sizeof(PROF_STR), "%d : %s %d",      RMXOS::getTick(), __FILE__, __LINE__);     DEBUGMSG("Profile", PROF_STR); }
#  define PROFILE_STR(x) { char PROF_STR[256]=""; VPRINTF(PROF_STR, sizeof(PROF_STR), "%d : %s %d : %s", RMXOS::getTick(), __FILE__, __LINE__, x);  DEBUGMSG("Profile", PROF_STR); }
#  define PROFILE_INT(x) { char PROF_STR[256]=""; VPRINTF(PROF_STR, sizeof(PROF_STR), "%d : %s %d : %d", RMXOS::getTick(), __FILE__, __LINE__, x);  DEBUGMSG("Profile", PROF_STR); }
#else
#  define DEBUGMSG(name, msg)
#  define PROFILE()
#  define PROFILE_STR(x)
#  define PROFILE_INT(x)
#endif //_RMX_DEBUG_


#ifdef RMX_MEDIAEDITION
#  define RMX_WINAMP         //Nullsoft Winamp support
//#  define RMX_WMP            //Microsoft Windows Media Player support
#  define RMX_RADLIGHT       //RadLight Player support
#  define RMX_BSP            //BSplayer support
#  if (_MSC_VER > 1200)      //the foobar2000 SDK will not compile on Visual Studio 6
#    define RMX_FOOBAR2000   //foobar2000 support (v1.x and higher)
#  endif
#endif


#endif //_RMX_PREDEFINES_
