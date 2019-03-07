/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "Win32.h"


HINSTANCE vWin32::hInstance = NULL;
OSVERSIONINFO vWin32::windowsversion = {0};
char  vWin32::filePath[MAX_PATH] = "";
char *vWin32::fileName = NULL;


int vWin32::getOsVersionMajor()  {
  if (windowsversion.dwOSVersionInfoSize == 0) updateOsInfo();
  return windowsversion.dwMajorVersion;
}


int vWin32::getOsVersionMinor() {
  if (windowsversion.dwOSVersionInfoSize == 0) updateOsInfo();
  return windowsversion.dwMinorVersion;
}


int vWin32::getOsVersionBuild() {
  if (windowsversion.dwOSVersionInfoSize == 0) updateOsInfo();
  return windowsversion.dwBuildNumber;
}


int vWin32::getOsVersionPlatform() {
  if (windowsversion.dwOSVersionInfoSize == 0) updateOsInfo();
  return windowsversion.dwPlatformId;
}


BOOL vWin32::isWin9x() {
  return (getOsVersionPlatform() == 1);
}


BOOL vWin32::isWinNT() {
  return (getOsVersionPlatform() == 2);
}


BOOL vWin32::isWin50() {
  if (isWin9x()) return FALSE;
  return (getOsVersionMajor() >= 5);
}


BOOL vWin32::isVista() {
  if (isWin9x()) return FALSE;
  return (getOsVersionMajor() >= 6);
}


BOOL vWin32::isWinNT4() {
  if (isWin9x()) return FALSE;
  return (getOsVersionMajor() == 4);
}


BOOL vWin32::isWinXP() {
  if (getOsVersionMajor() > 5) return TRUE;
  if (getOsVersionMajor() == 5) return (getOsVersionMinor() > 0);
  return FALSE;
}


void vWin32::updateOsInfo() {
  ZeroMemory(&windowsversion, sizeof(OSVERSIONINFO));
  windowsversion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&windowsversion);
}


const char *vWin32::getPath() {
  if (*filePath) return filePath;
  ZeroMemory(filePath, sizeof(filePath));
  
  wchar_t PATH[MAX_PATH] = L"";
  GetModuleFileName(getInstance(), PATH, sizeof(PATH)/2);
  rmxToUtf8 str(PATH);

  VSTRCPY(filePath, str.getBuffer(), sizeof(filePath));

  char *p = filePath + VSTRLEN(filePath);
  while (p >= filePath && *p != '\\') p--;
  if (p >= filePath) *p = 0;
  fileName = p + 1;

  return filePath;
}


const char *vWin32::getName() {
  if (!fileName) getPath();
  return fileName;
}


UINT vWin32::getTick() {
  return GetTickCount();
}
