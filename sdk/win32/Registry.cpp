/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include <stdio.h>
#include <stdlib.h>

#include "../rmxBase.h"
#include "Registry.h"



bool vRegistry::regKeyString(HKEY hKey, const char *regPath, const char *regName, char *returnText) {
  HKEY curKey = NULL;
  char retText[MAX_PATH];
  unsigned long strLen = MAX_PATH;

  //TODO:  we need a brand new system registry class
  if (RegOpenKeyExA(hKey, regPath , 0, KEY_READ, &curKey) != ERROR_SUCCESS) return false;
  if (RegQueryValueExA(curKey, regName, NULL, NULL, (LPBYTE) retText, &strLen) != ERROR_SUCCESS) return false;
  RegCloseKey(curKey);

  VSTRCPY(returnText, retText, sizeof(retText));
  return true;
}