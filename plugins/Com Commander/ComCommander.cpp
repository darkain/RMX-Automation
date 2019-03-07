/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "ComCommander.H"


//static ComCommander ccmdr;
static ComCommander *ccmdr = new ComCommander();


HANDLE OpenCOMPort(int portnum) {
  HANDLE hCom;
  char port[8];

  SECURITY_ATTRIBUTES sa={sizeof(sa),};
  SECURITY_DESCRIPTOR sd={0,};
  if (vWin32::isWinNT()) {
    InitializeSecurityDescriptor(&sd,SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, true, NULL, false);
    sa.lpSecurityDescriptor = &sd;
  }
  else sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle = true;

  VPRINTF(port, sizeof(port), "COM%d", portnum);

  hCom = CreateFileA(port, GENERIC_READ | GENERIC_WRITE, 0, &sa, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

  if (hCom == INVALID_HANDLE_VALUE) return  NULL;
//  SetCommMask(hCom, EV_CTS|EV_DSR|EV_RING|EV_RLSD);
//  EscapeCommFunction (hCom, CLRRTS);
//  EscapeCommFunction (hCom, SETRTS);

  return hCom;
}



ComCommander::ComCommander() : plugBase("Com Commander") {
  root = NULL;
}

void ComCommander::onInit() {
  root = new ComTree(getName());
  root->init();
  root->Begin();
}

void ComCommander::onQuit() {
  if (!root) return;
  root->End();
  delete root;
  root = NULL;
}


ComTree::ComTree(const char *name) : cfgTree(name), vMessageWnd(name) {
  btn[0] = new evntPress("Button 1 (CTS)", this);
  btn[1] = new evntPress("Button 2 (DSR)", this);
  btn[2] = new evntPress("Button 3 (RLSD)", this);
  btn[3] = new evntPress("Button 4 (RING)", this);

  btn[0]->setHint("action0");
  btn[1]->setHint("action1");
  btn[2]->setHint("action2");
  btn[3]->setHint("action3");

  hcom = OpenCOMPort(3);
}


VUINT ComTree::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == WM_USER+1  &&  param1 < 4) {
    btn[param1]->setPress(param2);
  }
  return vMessageWnd::wndProc(msg, param1, param2);
}


int ComTree::threadProc() {
  DWORD dwModemStat=0;
  DWORD lastActive=0;

  while (isRunning()) {
    Sleep(1);

    if (GetCommModemStatus(hcom, &dwModemStat)) {
      if ( (lastActive & MS_CTS_ON) != (dwModemStat & MS_CTS_ON) ) {
        PostMessage(getWnd(), WM_USER+1, 0, (dwModemStat & MS_CTS_ON));
      }
      
      if ( (lastActive & MS_DSR_ON) != (dwModemStat & MS_DSR_ON) ) {
        PostMessage(getWnd(), WM_USER+1, 1, (dwModemStat & MS_DSR_ON));
      }
      
      if ( (lastActive & MS_RING_ON) != (dwModemStat & MS_RING_ON) ) {
        PostMessage(getWnd(), WM_USER+1, 2, (dwModemStat & MS_RING_ON));
      }

      if ( (lastActive & MS_RLSD_ON) != (dwModemStat & MS_RLSD_ON) ) {
        PostMessage(getWnd(), WM_USER+1, 3, (dwModemStat & MS_RLSD_ON));
      }

      lastActive = dwModemStat;
    }
  }
  
  setDeleteNow(TRUE);
  return getSelfDelete();
}
