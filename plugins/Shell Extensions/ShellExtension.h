/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


//TODO:  test for x64 compatibilities


#ifndef _SHELL_EXTENSION_H_
#define _SHELL_EXTENSION_H_


#include "../../sdk/plugBase.h"
#include "../../sdk/Win32/MessageWnd.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/evnt/evntSingle.h"
#include "../../sdk/func/funcSingle.h"


class vShellExtension : public plugBase {
  public:
    vShellExtension();

  protected:
    virtual void onInit();

  public:
    void win32Message(HPLUGIN source, HLIST input, HLIST output, int method);
    HWND getWindow(HPLUGIN source, HLIST input);
};


class vShellWnd : public vMessageWnd, public cfgTree {
  public:
    vShellWnd(const char *name, cfgBase *parent);
    virtual ~vShellWnd();

  protected:
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);
};


class FindWindowScript : public funcSingle {
  public:
    FindWindowScript(cfgRoot *parent) : funcSingle(NULL, "FindWindow", parent) {
//      setTooltip("http://google.com/search?btnI=1&q=FindWindow");
      new funcParam("f_class",  "f_class",  "string", this);
      new funcParam("f_window", "f_window", "string", this);
    }
    virtual ~FindWindowScript() {}

  public:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
      PROFILE_STR("FindWindow");
      rmxUtf8ToWide hclass(  source->list_getParam(input, "f_class" ) );
      rmxUtf8ToWide hwindow( source->list_getParam(input, "f_window") );
      source->list_insertParam( output, "prm1" );
      source->list_setParamInt( output, "prm1", (int)FindWindow(hclass.getBuffer(), hwindow.getBuffer()) );
    }
};


class FindWindowExScript : public funcSingle {
  public:
    FindWindowExScript(cfgRoot *parent) : funcSingle(NULL, "FindWindowEx", parent) {
//      setTooltip("http://google.com/search?btnI=1&q=FindWindowEx");
      new funcParam("f_hwnd",    "f_hwnd",    "int", this);
      new funcParam("f_hinsert", "f_hinsert", "int", this);
      new funcParam("f_class",   "f_class",   "string", this);
      new funcParam("f_window",  "f_window",  "string", this);
    }
    virtual ~FindWindowExScript() {}

  public:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
      PROFILE_STR("FindWindowEx");
      HWND hwnd   = (HWND) source->list_getParamLong(input, "f_hwnd");
      HWND insert = (HWND) source->list_getParamLong(input, "f_hinsert");
      rmxUtf8ToWide hclass(  source->list_getParam(input, "f_class" ) );
      rmxUtf8ToWide hwindow( source->list_getParam(input, "f_window") );
      source->list_insertParam(  output, "prm1" );
      source->list_setParamLong( output, "prm1", (RMXLONG)FindWindowEx(hwnd, insert, hclass.getBuffer(), hwindow.getBuffer()) );
    }
};


class SendMessageScript : public funcSingle {
  public:
    SendMessageScript(cfgRoot *parent) : funcSingle("Send Message", "SendMessage", parent) {
      PROFILE_STR("SendMessage");
//      setTooltip("http://google.com/search?btnI=1&q=SendMessage");
      new funcParam("Window",  "f_hwnd", "string", this);
      new funcParam("Message", "f_msg",  "int",    this);
      new funcParam("WParam",  "f_wprm", "int",    this);
      new funcParam("LParam",  "f_lprm", "int",    this);
    }
    virtual ~SendMessageScript() {}

  public:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


class PostMessageScript : public funcSingle {
  public:
    PostMessageScript(cfgRoot *parent) : funcSingle("Post Message", "PostMessage", parent) {
      PROFILE_STR("PostMessage");
//      setTooltip("http://google.com/search?btnI=1&q=PostMessage");
      new funcParam("Window",  "f_hwnd", "string", this);
      new funcParam("Message", "f_msg",  "int",    this);
      new funcParam("WParam",  "f_wprm", "int",    this);
      new funcParam("LParam",  "f_lprm", "int",    this);
    }
    virtual ~PostMessageScript() {}

  public:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};


class GetWindowTextScript : public funcSingle {
  public:
    GetWindowTextScript(cfgRoot *parent) : funcSingle(NULL, "GetWindowText", parent) {
//      setTooltip("http://google.com/search?btnI=1&q=GetWindowText");
      new funcParam("f_hwnd", "f_hwnd", "int", this);
    }
    virtual ~GetWindowTextScript() {}

  public:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);
};

/*
class vTaskSwitch : public funcSingle {
  public:
    vTaskSwitch(const char *name, UINT direction);
    virtual ~vTaskSwitch() {}

    virtual UINT onActivated(funcSingleCall *function);

    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
    BOOL enumProc(HWND hwnd);

  private:
    UINT dir;
    int pos;
    rmxList<HWND> wnds;
};


class vStartMenu : public funcSingle {
  public:
    vStartMenu() : funcSingle("Start Menu") {}
    virtual ~vStartMenu() {}
    virtual UINT onActivated(funcSingleCall *function);
};



class vPowerDvd : public funcSingle {
  public:
    vPowerDvd() : funcSingle("Power DVD - Play") {}
    virtual ~vPowerDvd() {}
    
    virtual UINT onActivated(funcSingleCall *function) {
      SendMessage(FindWindow("Class of CyberLink Universal Player", NULL), 0x0319, 0x00000000, 0x000E0000);
//      SendMessage(FindWindow("Class of CyberLink Universal Player", NULL), WM_USER+4864, 2, 1);
      return 0;
    }
};
*/

#endif //_SHELL_EXTENSION_H_
