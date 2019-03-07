/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _MAIN_H_
#define _MAIN_H_


#include "../vBase/DLL.h"
#include "../vBase/Thread.h"
#include "../vBase/Timer.h"
#include "../vBase/MessageWnd.h"
#include "../vBase/vConfigList/Config.h"
#include "../vBase/vConfigList/vEvent/Event.h"


class uitDevice;


class vInfra : public vPlug {
  public:
    vInfra();
    virtual ~vInfra();

    virtual void onInit();
    virtual void onQuit();

  private:
    vConfigTree *root;
    rmxList<uitDevice*> remotes;
};


class uitDevice : public vConfigTree, public vMessageWnd, public vThread {
  public:
    uitDevice(vConfigList *parent);
    virtual ~uitDevice();

    HANDLE openPort(int portnum);
    void closePort() { openPort(0); }

    BOOL setPortProperties(int baud, int parity=0, int byteSize=8, int stopBits=ONESTOPBIT, BOOL dtrControl=FALSE, BOOL rtsControl=FALSE);

  protected:
    virtual DWORD ThreadProc();
    virtual UINT wndProc(UINT msg, UINT param1, UINT param2);

  private:
    HANDLE hPort;
    DCB dcb;
};


class uitMessage : public vEvent, public vTimer {
  public:
    uitMessage(const char *name, UINT btnid, vConfigList *parent);
    virtual ~uitMessage() {}

  protected:
    virtual void onTimer(DWORD systemTime);
    virtual void onActivated(UINT retval);
};


#endif//_MAIN_H_
