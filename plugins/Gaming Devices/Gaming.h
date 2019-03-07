/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _GAMING_H_
#define _GAMING_H_


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/Win32/Timer.h"
#include "../../sdk/Win32/MessageWnd.h"
#include "../../sdk/evnt/evntSingle.h"
#include "../../sdk/rmxGuid.h"


#define DIRECTINPUT_VERSION 0x0700
#include <dinput.h>


class vGameDevice;


class vGaming : public plugBase {
  public:
    vGaming();
    virtual ~vGaming();

  public:
    BOOL detect();
    BOOL enumCallback(const DIDEVICEINSTANCE* instance);

    inline LPDIRECTINPUT7 getDi() { return di; }

    inline vMessageWnd *getWnd() { return wnd; }

    vGameDevice *getDevice(GUID guid);
    inline void appendDevice(vGameDevice *dev) { devices.appendItem(dev); }
    inline void removeDevice(vGameDevice *dev) { devices.removeItem(dev); }

  protected:
    virtual void onInit();
    virtual void onQuit();

  private:
    cfgBase *tree;
    vMessageWnd *wnd;

    LPDIRECTINPUT7 di;
    DWORD dxVersion;
    DWORD dxRevision;

    rmxList<vGameDevice*> devices;
};


extern vGaming *gaming;



class vGameTree : public cfgTree {
  public:
    vGameTree(const char *name);
    virtual ~vGameTree();

  protected:
    virtual cfgBase *onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist);
};


class vGameDetect : public cfgButton {
  public:
    vGameDetect(cfgBase *parent) : cfgButton("Scan for new Devices", parent) {
      setButtonText("Scan");
    }

    virtual ~vGameDetect() {}

  protected:
    virtual void onClick(HPLUGIN source);
};



#endif//_GAMING_H_