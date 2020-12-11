/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "Comm.h"


vComm::vComm() {
  hcomm = NULL;
  ring = FALSE;

  ZeroMemory(&dcb, sizeof(dcb));
  dcb.DCBlength = sizeof(DCB);
}


vComm::~vComm() {
  closePort();
}


BOOL vComm::openPort(const char *port) {
  hcomm = CreateFileA(port, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
  if (!hcomm) return FALSE;


  if (!SetCommMask(hcomm, EV_RXCHAR|EV_RING)) {
    char str[32]="";
    VPRINTF(str, sizeof(str), "ERROR: %08X", GetLastError());
    CONSOLE_MAIN->printError(port, str);
    return FALSE;
  }


  SetupComm(hcomm, 4096, 4096);

  purge();

  COMMTIMEOUTS timeout;
  ZeroMemory(&timeout, sizeof(timeout));
  timeout.ReadIntervalTimeout = 300;
  timeout.ReadTotalTimeoutMultiplier = 300;
  timeout.ReadTotalTimeoutConstant = 300;
  timeout.WriteTotalTimeoutMultiplier = 300;
  timeout.WriteTotalTimeoutConstant = 300;
  SetCommTimeouts(hcomm, &timeout);


  // set baud rate and comms  parameters
  dcb.BaudRate = CBR_4800;
  dcb.ByteSize = 8; // bits per byte
  dcb.Parity   = 0; // 0-4=no,odd,even,mark,space
  dcb.StopBits = 0; // 0,1,2 = 1, 1.5, 2

  // setup hardware flow control
  dcb.fDtrControl = DTR_CONTROL_ENABLE;
  dcb.fRtsControl = RTS_CONTROL_ENABLE;

  // setup software flow control
  dcb.fInX     = TRUE;
  dcb.fOutX    = TRUE;
  dcb.XonChar  = ASCII_XON;
  dcb.XoffChar = ASCII_XOFF;
  dcb.XonLim   = 100;
  dcb.XoffLim  = 100;

  // other various settings
  dcb.fBinary = TRUE;
  dcb.fParity = TRUE;

  SetCommState(hcomm, &dcb);

  EscapeCommFunction(hcomm, SETDTR);

  Begin();
  startTimer();
  return TRUE;
}


void vComm::closePort() {
  if (!hcomm) return;

  CloseHandle(hcomm);
  hcomm = NULL;

  stopTimer();
  End();
}


BOOL vComm::readBytes(unsigned char *data, int len) {
  if (!hcomm) return FALSE;

  DWORD read = 0;
  BOOL ret = FALSE;

  OVERLAPPED overlapped = {0};
  overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (overlapped.hEvent == NULL) return FALSE;

  if (!ReadFile(hcomm, data, len, &read, &overlapped)) {
    if (GetLastError() == ERROR_IO_PENDING) {
      if (WaitForSingleObject(overlapped.hEvent, READ_TIMEOUT) == WAIT_OBJECT_0) {
        ret = !!GetOverlappedResult(hcomm, &overlapped, &read, FALSE);
      }
    }
  } else {    
    ret = TRUE;
  }

  CloseHandle(overlapped.hEvent);
  return ret;
}


BOOL vComm::writeBytes(const unsigned char *data, int len) {
  if (!hcomm) return FALSE;

  DWORD written = 0;
  BOOL ret = FALSE;

  OVERLAPPED overlapped = {0};
  overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (overlapped.hEvent == NULL) return FALSE;

  if (!WriteFile(hcomm, data, len, &written, &overlapped)) {
    if (GetLastError() == ERROR_IO_PENDING) { 
      if (WaitForSingleObject(overlapped.hEvent, INFINITE) == WAIT_OBJECT_0) {
        ret = !!GetOverlappedResult(hcomm, &overlapped, &written, FALSE);
      }
    }
  } else {
    ret = TRUE;
  }

  CloseHandle(overlapped.hEvent);
  return ret;
}


void vComm::purge() {
  if (!hcomm) return;
  PurgeComm(hcomm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
}


int vComm::threadProc() {
  DWORD status = 0;

  OVERLAPPED overlapped = {0};
  overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (overlapped.hEvent == NULL) return FALSE;

  while (isRunning()) {
    status = 0;
    if (WaitCommEvent(hcomm, &status, &overlapped)) {
      if((status & EV_RING) && !isRinging()) {
        lock();
        ring = TRUE;
        unlock();
      }
    } else {
      if (GetLastError() == ERROR_IO_PENDING) {
        if (WaitForSingleObject(overlapped.hEvent, INFINITE) == WAIT_OBJECT_0) {
          if (GetOverlappedResult(hcomm, &overlapped, &status, TRUE)) {
            if (!isRinging()) {
              lock();
              ring = TRUE;
              unlock();
            }
          }
        }

      }
    }
    Sleep(1);
  }
  
  return false;
}


void vComm::onTimer(DWORD systemTime) {
  if (isRinging()) {
    onRing();
    lock();
    ring = FALSE;
    unlock();
  }
}

