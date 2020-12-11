/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _PWR_H_
#define _PWR_H_


#include "../../sdk/plugBase.h"
#include "../../sdk/Win32/Timer.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/func/funcSingle.h"
#include "../../sdk/func/funcGroup.h"
#include "../../sdk/evnt/evntPress.h"


class vPowerManagement : public plugBase {
  public:
    vPowerManagement();

    virtual void onInit();
    virtual void onQuit();

    BOOL GetShutdownPrivilege();

  private:
    funcGroup *functions;
    cfgTree    *battery;
    cfgTree    *tree;
};


class vSystemPowerState : public funcSingle {
  public:
    vSystemPowerState(const char *function_name, const char *script_name, funcGroup *parent, BOOL suspendState);
    virtual ~vSystemPowerState() {}

    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    BOOL pwrstate;
};


class vSystemShutdown : public funcSingle {
  public:
    vSystemShutdown(const char *function_name, const char *script_name, funcGroup *parent, BOOL powerState);
    virtual ~vSystemShutdown() {}

    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    BOOL pwrstate;
};


class vRemoteShutdown : public funcSingle {
  public:
    vRemoteShutdown(const char *function_name, const char *script_name, funcGroup *parent, BOOL powerState);
    virtual ~vRemoteShutdown() {}

    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    BOOL pwrstate;
};


class vBattery : public cfgTree, public vTimer {
  public:
    vBattery();
    virtual ~vBattery();

    virtual void onTimer(DWORD systemTime);

    const char *getFormattedTime(int sec);

  private:
    cfgText *ac;
    cfgBool *charge;
    cfgInt  *percent;
    cfgText *time;
    cfgText *totaltime;
    cfgText *status;
    char buffer[32];
};

/*
class rmxLockWorkstation : public funcSingle {
  public:
    rmxLockWorkstation(const char *name, funcGroup *parent);
    virtual ~rmxLockWorkstation() {}

    virtual UINT onActivated(funcSingleCall *function);
};
*/


class vMonitor : public funcSingle {
  public:
    vMonitor(const char *function_name, const char *script_name, UINT moncommand, UINT monparam, funcGroup *parent);
    virtual ~vMonitor() {}
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    UINT command;
    UINT param;
};


class vDetectScreenSaver : public cfgTree, public vTimer {
  public:
    vDetectScreenSaver();
    virtual ~vDetectScreenSaver() {}

  protected:
    virtual void onTimer(DWORD systemTime);

    BOOL getScreenSaver();

  private:
    evntPress *event;
    BOOL last;
};


#endif //_PWR_H_
