/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WAMAIN_H_
#define _WAMAIN_H_


#include "../../../sdk/func/funcSingle.h"
#include "../../../sdk/func/funcGroup.h"
#include "../../../sdk/cfg/config.h"
#include "../../rmx_win32/rmx_win32.h"
#include "../common.h"
#include "gen.h"


class rmxUdpRemote;
class waPlugins;
class waVolume;
class waBalance;
class waEqBand;
class waEqBool;
extern waPlugins *waRmx;


class waPlugins : public vRmxWin32 {
  public:
    waPlugins();
    virtual ~waPlugins();

    virtual void onInit();
    virtual void initMasterLists(vLocalPlugin *master);

    virtual const char *getHostName();

    inline HWND getWinamp() const { return hwinamp; }
    inline void setWinamp(HWND hwnd) { hwinamp = hwnd; }

    void closeWinamp();
    UINT winampcommand(UINT command) const;
    UINT winampmessage(UINT command, UINT param=0) const;


    static LRESULT CALLBACK static_WinampProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
    virtual LRESULT CALLBACK WinampProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

  private:
    WNDPROC prevWndProc;
    HWND hwinamp;

	rmxUdpRemote *udpremote;

    waVolume *wavolume;
    waBalance *wabalance;

    waEqBool *eq;
    waEqBool *autoeq;
    rmxList<waEqBand*> waeq;
};


class waCommand : public funcSingle {
  public:
    waCommand(const char *function_name, const char *script_name, UINT command, funcGroup *parent);
    waCommand(const char *function_name, const char *script_name, UINT command, cfgRoot *parent);
    virtual ~waCommand() {}
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


class waMessage : public funcSingle {
  public:
    waMessage(const char *function_name, const char *script_name, UINT command, UINT value, funcGroup *parent);
    virtual ~waMessage() {}
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


class waToggleMessage : public funcSingle {
  public:
    waToggleMessage(const char *function_name, const char *script_name, UINT command, UINT request, funcGroup *parent);
    virtual ~waToggleMessage() {}
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


class waToggleWindow : public funcSingle {
  public:
    waToggleWindow(const char *function_name, const char *script_name, UINT command, UINT wnd_id, UINT toggle, funcGroup *parent);
    virtual ~waToggleWindow() {}
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};



class waCloseWinamp : public funcSingle {
  public:
    waCloseWinamp(cfgRoot *parent) : funcSingle("Close Winamp", "close", parent) { setHint("closeapp"); }
    virtual ~waCloseWinamp() {}
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};



class waEqBand : public cfgIntSlider {
  public:
    waEqBand(const char *name, cfgBase *parent, int eqband);
    virtual ~waEqBand() {}

    virtual void onSetUserData(const prmBase *userdata);
    void update();

  private:
    int band;
    BOOL lock;
};


class waEqBool : public cfgBool {
  public:
    waEqBool(const char *name, cfgBase *parent, int eqband);
    virtual ~waEqBool() {}

    virtual void onSetUserData(const prmBase *userdata);
    void update();

  private:
    int band;
    BOOL lock;
};


class waVolume : public cfgIntSlider {
  public:
    waVolume(cfgBase *parent);
    virtual ~waVolume() {}

    virtual void onSetUserData(const prmBase *userdata);
    void update();

  private:
    BOOL lock;
};


class waBalance : public cfgIntSlider {
  public:
    waBalance(cfgBase *parent);
    virtual ~waBalance() {}

    virtual void onSetUserData(const prmBase *userdata);
    void update();

  private:
    BOOL lock;
};





class waJump : public funcSingle {
  public:
    waJump(const char *function_name, const char *script_name, UINT id, cfgRoot *parent);
    virtual ~waJump() {}

    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    UINT jid;
};


#endif //_WAMAIN_H_
