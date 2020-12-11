/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _INTELLIREMOTE_H_
#define _INTELLIREMOTE_H_


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/evnt/evntSingle.h"
#include "../../sdk/Win32/MessageWnd.h"
#include "../../sdk/Win32/Timer.h"
#include <mmsystem.h>



class vIntelliRemote : public plugBase {
  public:
    vIntelliRemote();

  protected:
    virtual void onInit();
    virtual void onPostInit();
    virtual void onQuit();

  public:
    evntSingle *getEvent(const char *button, cfgBase *parent=NULL);

    void start();
    void stop();
    
    void handleMidiData(UINT param1, UINT param2);

  protected:  
    int  handleRemoteMem(char* c, HMIDIIN hMidi);
    void handleButton(UINT ID);
    void setDeviceNumber(int devNum);
    int  detectDevice();

    inline int  getDeviceNumber() { return irDevNum; }
    inline BOOL getIrDetected()   { return irFailed; }

  private:
    char buffer[32];

    cfgTree *tree;
    cfgText *log;
    cfgNull *deviceid;
    cfgNull *devicename;

    vMessageWnd *wnd;

    MIDIHDR *irMidiHdr;
    HMIDIIN irHandle;
    int     irDevNum;
    BOOL    irFailed;
    BOOL    isLoading;
    MIDIINCAPS mic;
};



class vIntelliRemoteWnd : public vMessageWnd {
  public:
    vIntelliRemoteWnd();
    virtual ~vIntelliRemoteWnd();

  protected:
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);
};



class vRemoteEvent : public evntSingle {
  public:
    vRemoteEvent(const char *initname, cfgBase *parent, const char *key, const char *hint=NULL)
      : evntSingle(initname, parent) {

      (insertParamNoSave("key", key))->setReadOnly(TRUE);
      if (hint) setHint(hint);
    }

    virtual ~vRemoteEvent() {}
};


#endif //_INTELLIREMOTE_H_
