/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __WIN32_H_
#define __WIN32_H_


#include "../rmxOS.h"


class vWin32 {
  private:
    vWin32() {}
    ~vWin32() {}

  public:
    //hInstance is automatically set by the RMX Plugin Loader, but must be manually set
    //if not being used with RMX, such as for a stand-alone application outside of RMX.
    //hInstance is made static, as there is only one per EXE/DLL file
    inline static HINSTANCE getInstance() { return hInstance; }
    inline static void setInstance(HINSTANCE hInst) { hInstance = hInst; }

    /*
    Major returns OS major version:
      4 = 95, 98, ME, NT4
      5 = 2000, XP, 2003
      6 = Vista, 7 (yes, Windows 7 returns version 6.1)

    Platform returns 1 for isWin9x(), 2 for isWinNT()
    */
    static int getOsVersionMajor();
    static int getOsVersionMinor();
    static int getOsVersionBuild();
    static int getOsVersionPlatform();

    /*
    isWin9x() returns true on all versions of Windows 95, 98, and ME
    isWinNT() returns true on all versions of Windows NT, including 2000, XP, 2003, and Vista
    isWin50() returns true on all versions 5.0 and higher, including 2000, XP, 2003, and Vista
    isWinNT4() returns true ONLY for Windows NT 4.0 (any service pack)
    isWinXP() returns true on all versions XP or higher, including 2003 and Vista
    isVista() return true on Windows Vista or higher
    */
    static BOOL isWin9x();
    static BOOL isWinNT();
    static BOOL isWin50();
    static BOOL isWinNT4();
    static BOOL isWinXP();
    static BOOL isVista();


    static const char *getPath();
    static const char *getName();

    static UINT getTick();

  private:
    static void updateOsInfo();

  private:
    static HINSTANCE hInstance;
    static OSVERSIONINFO windowsversion;

    static char  filePath[MAX_PATH];
    static char *fileName;
};


#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam) ((signed short)(LOWORD(lParam)))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam) ((signed short)(HIWORD(lParam)))
#endif


#endif //__WIN32_H_
