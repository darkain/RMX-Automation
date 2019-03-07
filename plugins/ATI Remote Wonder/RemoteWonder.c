/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "ammo.h"


#define _CLASS_NAME "ATI RemoteWonder for RMX"


DWORD WhatKeysDoYouWant(void) {
  return (0x7FFF);
}


char *EnumerateProgrammableFunction(WORD wIndex) {  
  return NULL;
}


void Configure(HANDLE hAti) {
  HWND hWnd = FindWindow(_CLASS_NAME, _CLASS_NAME);
  if (hWnd) {
    SendMessage(hWnd, WM_USER+327, 256, 0);
  } else  {
    MessageBox(hAti, "RMX Automation not found.", _CLASS_NAME, MB_OK);
  }
}


int AreYouInFocus(void) {
  HWND hWnd = FindWindow(_CLASS_NAME, _CLASS_NAME);
  return (hWnd != NULL);
}


BOOL HandleKey(BOOL bCustom, WORD wKeyEvent, WORD wState) {
  HWND hWnd = FindWindow(_CLASS_NAME, _CLASS_NAME);
  if (hWnd == NULL) return FALSE;

  if (wState == RMCTRL_KEY_ON) {
    PostMessage(hWnd, WM_USER+328, wKeyEvent, 1);
    return TRUE;
  }

  else if (wState == RMCTRL_KEY_OFF) {
    PostMessage(hWnd, WM_USER+328, wKeyEvent, 0);
    return TRUE;
  }

/*
  else if (wState == RMCTRL_KEY_REPEAT) {
    switch (wKeyEvent) {
      case RMCTRL_VOLUMEUP:   
      case RMCTRL_VOLUMEDOWN:
        PostMessage(hWnd, WM_USER+328, wKeyEvent, 0);
        break;
      default:  return FALSE;
    }
    return TRUE;
  }
*/
  return FALSE;
}


static BOOL HandleProgrammedKey(HWND hWinamp, WORD wKeyEvent, WORD wState) {
  return FALSE;
}
