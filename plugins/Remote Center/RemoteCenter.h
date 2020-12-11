/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _REMOTECENTER_H_
#define _REMOTECENTER_H_

#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "shell32.lib")


#include "../../sdk/plugBase.h"
#include "../../sdk/Win32/Registry.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/evnt/evntSingle.h"
#include "../../sdk/wnd/wndBase.h"


class vRemoteCenter : public plugBase, public vRegistry {
  public:
    vRemoteCenter();
    virtual ~vRemoteCenter();

  protected:
    virtual void onInit();
    virtual void onQuit();

  public:
    void onCommand(int id);

  private:
    void EditConfig(char *path);
    void MakeKeyFile();
    BOOL BuildConfig();
    bool GetPath(char *Path);

    void writeConfig(UINT id, const char *name, const char *file);


  private:
    cfgTree *tree;
    vMessageWnd *wnd;

    UINT rcMajor;
    UINT rcMinor;
    UINT rcRevision;
    BOOL rcFound;
};



class vRemoteCenterWnd : public wndBase {
  public:
    vRemoteCenterWnd();
    virtual ~vRemoteCenterWnd();
    virtual void onInit();
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);
};



class vRemoteEvent : public evntSingle {
  public:
    vRemoteEvent(const char *initname, cfgBase *parent, int cmd, const char *hint=NULL)
      : evntSingle(initname, parent) {

      insertParamIntNoSave("cmd", cmd);
      setParamReadOnly("cmd", TRUE);
      if (hint) setHint(hint);
    }

    virtual ~vRemoteEvent() {}
};


#endif//_REMOTECENTER_H_
