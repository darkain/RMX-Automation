/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#error UPDATE ME TO SOMETHING THAT ISNT THREE YEARS OLD

#include <stdio.h>
#include "Main.H"

static vInfra infra;

vEvent *numbers[16];
vEvent *playback[15];
vEvent *misc[11];

/*
unsigned char buttons[][6] = {
  {0x4D, 0x05, 0x67, 0x98, 0xAC, 0x21}, //play 0
  {0x4D, 0x05, 0x27, 0xD8, 0xAC, 0x21}, //pause 1
  {0x4D, 0x05, 0xA7, 0x58, 0xAC, 0x21}, //stop 2
  {0x4D, 0x05, 0xC7, 0x38, 0xAC, 0x21}, //eject 3
  {0x4D, 0x05, 0x47, 0xB8, 0xAC, 0x21}, //prev track 4
  {0x4D, 0x05, 0xFB, 0x04, 0xAC, 0x21}, //next track 5
  {0x4D, 0x05, 0x87, 0x78, 0xAC, 0x21}, //fast rewind 6
  {0x4D, 0x05, 0x07, 0xF8, 0xAC, 0x21}, //fast forward 7
  {0x4D, 0x05, 0x8B, 0x74, 0xAC, 0x21}, //0 8
  {0x4D, 0x05, 0x7B, 0x84, 0xAC, 0x21}, //1 9
  {0x4D, 0x05, 0xBB, 0x44, 0xAC, 0x21}, //2 10
  {0x4D, 0x05, 0x3B, 0xC4, 0xAC, 0x21}, //3 11
  {0x4D, 0x05, 0x5B, 0xA4, 0xAC, 0x21}, //4 12
  {0x4D, 0x05, 0x9B, 0x64, 0xAC, 0x21}, //5 13
  {0x4D, 0x05, 0x1B, 0xE4, 0xAC, 0x21}, //6 14
  {0x4D, 0x05, 0x6B, 0x94, 0xAC, 0x21}, //7 15
  {0x4D, 0x05, 0xAB, 0x54, 0xAC, 0x21}, //8 16
  {0x4D, 0x05, 0x2B, 0xD4, 0xAC, 0x21}, //9 17
  {0x4D, 0x05, 0xDB, 0x24, 0xAC, 0x21}, //Shift 18
  {0x4D, 0x05, 0xEB, 0x14, 0xAC, 0x21}, //Mouse Mode 19
  {0x4D, 0x05, 0xCB, 0x34, 0xAC, 0x21}, //Vol Up 20
  {0x4D, 0x05, 0xF3, 0x0C, 0xAC, 0x21}, //Vol Down 21
  {0x4D, 0x05, 0x0B, 0xF4, 0xAC, 0x21}, //Vol Mute 22
  {0x4D, 0x05, 0x4B, 0xB4, 0xAC, 0x21}, //Start 23
};
*/

vInfra::vInfra() : vPlug("Universal Infrared Transceiver") {
}


vInfra::~vInfra() {
}


void vInfra::onInit() {
  consoleMessage("[UIT] Starting");

  root = new vConfigTree(getName());
  root->setTitle("Universal Infrared Transceiver");

  uitDevice *device = new uitDevice(root);
  remotes.appendItem(device);
  device->init();
  device->Begin();
}



void vInfra::onQuit() {
  delete root;
  root = NULL;
}


//--------------------------------------------------------------


uitDevice::uitDevice(vConfigList *parent) : vConfigTree("Input Device 1", parent), vMessageWnd("UIT") {
  hPort = NULL;
  ZeroMemory(&dcb, sizeof(DCB));
  dcb.DCBlength = sizeof(DCB);

  new uitMessage("Play",           0x9867, this);
  new uitMessage("Stop",           0x58A7, this);
  new uitMessage("Pause",          0xD827, this);
  new uitMessage("Eject",          0x38C7, this);
  new uitMessage("Previous Track", 0xB847, this);
  new uitMessage("Next Track",     0x04FB, this);
  new uitMessage("Fast Rewind",    0x7887, this);
  new uitMessage("Fast Forward",   0xF807, this);
  new uitMessage("Shift",          0x24DB, this);
  new uitMessage("Mouse",          0x14EB, this);
  new uitMessage("Volume Up",      0x34CB, this);
  new uitMessage("Volume Down",    0x0CF3, this);
  new uitMessage("Volume Mute",    0xF40B, this);
  new uitMessage("Start",          0xB44B, this);
  new uitMessage("Button 1",       0x847B, this);
  new uitMessage("Button 2",       0x44BB, this);
  new uitMessage("Button 3",       0xC43B, this);
  new uitMessage("Button 4",       0xA45B, this);
  new uitMessage("Button 5",       0x649B, this);
  new uitMessage("Button 6",       0xE41B, this);
  new uitMessage("Button 7",       0x946B, this);
  new uitMessage("Button 8",       0x54AB, this);
  new uitMessage("Button 9",       0xD42B, this);
  new uitMessage("Button 0",       0x748B, this);

  openPort(2);
  setPortProperties(2400);
  EscapeCommFunction(hPort, SETDTR);
  EscapeCommFunction(hPort, SETRTS);
}


uitDevice::~uitDevice() {
  End();
  closePort();
}


UINT uitDevice::wndProc(UINT msg, UINT param1, UINT param2) {
  if (msg == WM_USER+1) if (param1 > 0) {
    for (vConfigList *list=getFirstChild(); list; list=getNextChild(list)) {
      if (((int)LOWORD(param1)) == list->getParamInt("BtnId")) {
        static_cast<vEvent*>(list)->activate();
        return 0;
      }
    }

    char str[256];
    sprintf(str, "[UIT] signal(0x%04X, 0x%04X, 0x%04X);", LOWORD(param2), LOWORD(param1), HIWORD(param2));
    infra.consoleMessage(str);
  }
  return vMessageWnd::wndProc(msg, param1, param2);
}


DWORD uitDevice::ThreadProc() {
  DWORD dwModemStat=0;
  DWORD lastActive=0;
  DWORD read=0;
  DWORD status=0;
  BYTE data[6];

  while (running) {
    Sleep(1);
    if (GetCommModemStatus(hPort, &status)) {
      WaitCommEvent(hPort, &status, 0);
      ZeroMemory(data, sizeof(data));
      ReadFile (hPort, data, 6, &read, 0);
      if (read > 0) {
        USHORT pre = *(USHORT*)&data;
        USHORT mid = *(USHORT*)&data[2];
        USHORT post = *(USHORT*)&data[4];
        postMessage(WM_USER+1, mid, MAKELONG(pre, post));
      }
    }
  }
  
  selfDelNow=TRUE;
  return selfDel;
}


HANDLE uitDevice::openPort(int portnum) {
  if (hPort) CloseHandle(hPort);
  if (portnum == 0) return NULL;

  char port[8];

  SECURITY_ATTRIBUTES sa={sizeof(sa),};
  SECURITY_DESCRIPTOR sd={0,};
  if (vBaseApp::isWinNT()) {
    InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, true, NULL, false);
    sa.lpSecurityDescriptor = &sd;
  }
  else sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle = true;

  wsprintf(port, "COM%d", portnum);

  hPort = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, &sa, OPEN_EXISTING, 0, NULL);
  if (hPort == INVALID_HANDLE_VALUE) hPort = NULL;
  return hPort;
}


BOOL uitDevice::setPortProperties(int baud, int parity, int byteSize, int stopBits, BOOL dtrControl, BOOL rtsControl) {
  if (!hPort) return FALSE;

  dcb.DCBlength = sizeof(DCB);     
  GetCommState(hPort, &dcb);

  dcb.BaudRate = baud;              // Current baud 
  dcb.fBinary = TRUE;               // Binary mode; no EOF check 

  dcb.fOutxCtsFlow = FALSE;         // No CTS output flow control 
  dcb.fOutxDsrFlow = FALSE;         // No DSR output flow control 
  dcb.fDtrControl = DTR_CONTROL_DISABLE; 

  dcb.fParity = (parity != 0);
  dcb.Parity = parity;

  dcb.fDsrSensitivity = FALSE;      // DSR sensitivity 
  dcb.fTXContinueOnXoff = FALSE;     // XOFF continues Tx 
  dcb.fOutX = FALSE;                // No XON/XOFF out flow control 
  dcb.fInX = FALSE;                 // No XON/XOFF in flow control 
  dcb.fErrorChar = FALSE;           // Disable error replacement 
  dcb.fNull = FALSE;                // Disable null stripping 
  dcb.fRtsControl = RTS_CONTROL_DISABLE; 
                                        // RTS flow control 
  dcb.fAbortOnError = FALSE;        // Do not abort reads/writes on 
                                        // error
  dcb.ByteSize = byteSize;                 // Number of bits/byte, 4-8 
  dcb.StopBits = stopBits;        // 0,1,2 = 1, 1.5, 2 

  if (!SetCommState (hPort, &dcb)) {
    infra.consoleMessage("[UIT] Unable to configure the COM port", -2);
    return FALSE;
  }

  COMMTIMEOUTS timeouts;
  ZeroMemory(&timeouts, sizeof(COMMTIMEOUTS));
  timeouts.ReadTotalTimeoutConstant = 24;
  SetCommTimeouts(hPort, &timeouts);
  
  return TRUE;  
}



//--------------------------------------------------------------


uitMessage::uitMessage(const char *name, UINT btnid, vConfigList *parent) : vEvent(name, parent), vTimer(500, FALSE) {
  insertParamIntNoCfg("button", btnid);
}


void uitMessage::onTimer(DWORD systemTime) {
  setHilighted(FALSE);
  stopTimer();
}


void uitMessage::onActivated(UINT retval) {
  setHilighted(TRUE);
  startTimer();
}
