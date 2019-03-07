/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WII_REMOTE_H_
#define _WII_REMOTE_H_


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/wnd/wndBase.h"
#include "../../sdk/Win32/Timer.h"
#include "../../sdk/evnt/evntPress.h"
#include "../../sdk/evnt/evntRelative.h"
#include "../../sdk/evnt/evntAbsolute.h"
#include "hidWiiEnum.h"
#include "hidWiiRemote.h"



class plugWii : public plugBase, public hidWiiEnum/*, public vTimer*/ {
  public:
    plugWii();

  protected:
    virtual void onInit();
    virtual void onQuit();

    virtual void onTimer(DWORD systemTime) { enumerate(); }
    virtual hidBase *enumDevice(HANDLE devhandle, HIDD_ATTRIBUTES *devattrib, const char *path);
};


//---------------------------------------------------------------------


class wiiRemote : public hidWiiRemote, public cfgTree {
  public:
    wiiRemote(HANDLE devhandle, HIDD_ATTRIBUTES *devattrib, const char *devpath);
    virtual ~wiiRemote();

  protected:
    virtual void onReadReport(unsigned char *data, int len);

    virtual void onAttachmentChange(unsigned char attachment);

    virtual void onButtons(unsigned short buttondata);
    virtual void onAccelData(unsigned char accl1, unsigned char accl2, unsigned char accl3);
    virtual void onIrData(unsigned short x, unsigned short y);

    virtual void nunchuk_onButtons(unsigned char buttondata);
    virtual void nunchuk_onAccelData(unsigned char accl1, unsigned char accl2, unsigned char accl3);
    virtual void nunchuk_onAnalogData(unsigned char x, unsigned char y);

    virtual void classic_onButtons(unsigned short buttondata);
    virtual void classic_onAnalogDataL(unsigned char x, unsigned char y);  //left analog stick
    virtual void classic_onAnalogDataR(unsigned char x, unsigned char y);  //right analog stick
    virtual void classic_onAnalogDataT(unsigned char l, unsigned char r);  //left and right analog triggers


  private:
    cfgNull      *rawdata;

    evntPress    *buttons[16];
    evntAbsolute *cursor[  2];
    evntRelative *remaccl[ 3];

    cfgTree      *nunchuk;
    evntPress    *nunbtns[2];
    evntRelative *nunaxis[2];
    evntRelative *nunaccl[3];

    cfgTree      *classic;
    evntPress    *cscbtns[16];
    evntRelative *cscaxis[ 4];
    evntAbsolute *csctrig[ 2];
};


/*
class wiiRemote : public cfgTree, public wndBase, public vThread, public vTimer {
  public:
    wiiRemote(const char *name, cfgBase *parent);
    virtual ~wiiRemote();

  protected:
    virtual void onTimer(DWORD systemTime);
    virtual DWORD ThreadProc();
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);

  public:
    void updateLights();
    void checkExt();

    BOOL readRequest(unsigned int addr, unsigned short bytes);
    
    BOOL writeData(unsigned int addr, unsigned char *indata, unsigned char bytes);
    BOOL writeByte(unsigned int addr, unsigned char byte);
    BOOL reportByte(unsigned char report, unsigned char byte);
    
    BOOL setInputMode(unsigned char mode);
    
    void enableCamera();

  private:
    void processInput(unsigned char *data);

  private:
//    hidBase hid;

    cfgNull   *button_text;
    evntPress *buttons[16];
    cfgInt    *sliders[ 6];
    cfgBool   *lights[  4];

    cfgInt       *camera[2];
    evntAbsolute *cursor[2];
    evntRelative *remaccl[3];

    cfgTree      *nunchuk;
    evntPress    *nunbtns[2];
    evntRelative *nunaxis[2];
    evntRelative *nunaccl[3];

    cfgTree      *classic;
    evntPress    *cscbtns[16];
    evntRelative *cscaxis[ 4];
    evntAbsolute *csctrig[ 2];

    unsigned char extmode;

    rmxList<unsigned char*> buffers;
};


//---------------------------------------------------------------------


class wiiLight : public cfgBool {
  public:
    wiiLight(const char *name, wiiRemote *parent) : cfgBool(name, parent), par(parent) {}

    virtual void onSetUserData(const prmBase *userdata) {
      cfgBool::onSetUserData(userdata);
      par->updateLights();
    }

  private:
    wiiRemote *par;
};


//---------------------------------------------------------------------


class wiiButton : public cfgButton {
  public:
    wiiButton(wiiRemote *parent) : cfgButton("check", parent), par(parent) {}

    virtual void onClick(HPLUGIN source) { par->checkExt(); }

  private:
    wiiRemote *par;
};
*/

#endif //_WII_REMOTE_H_
