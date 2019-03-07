/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _MESSAGEWND_H_
#define _MESSAGEWND_H_


#include "Win32.h"
#include "../rmxBase.h"
#include "../rmxString.h"
#include "../rmxList.h"
#include "../rmxName.h"


class wndBase;
class wndSubclass;
class wndDialog;


class vMessageWnd : public rmxString {
  friend class wndBase;
  friend class wndSubclass;
  friend class wndDialog;

  public:
    vMessageWnd(const char *Classname, vMessageWnd *parentwnd=NULL);
    vMessageWnd(const wchar_t *Classname, vMessageWnd *parentwnd=NULL);
    virtual ~vMessageWnd();

  public:
    //basic class functions
    HWND init();
	inline HWND getWnd()               const { return hwnd;       }
    inline vMessageWnd *getParentWnd() const { return parent;     }
    inline void setParentHWnd(HWND hParent)  { hparent = hParent; }
    inline const char *getClassName()  const { return classname;  }
    inline void setClassName(const char *c)  { classname = c;     }
    //end basic class functions

    
    //window messaging
    VUINT sendMessage( UINT msg, VUINT param1=0, VUINT param2=0);
    VUINT sendMessageA(UINT msg, VUINT param1=0, VUINT param2=0);
    VUINT sendMessageW(UINT msg, VUINT param1=0, VUINT param2=0);

    VUINT postMessage( UINT msg, VUINT param1=0, VUINT param2=0);
    VUINT postMessageA(UINT msg, VUINT param1=0, VUINT param2=0);
    VUINT postMessageW(UINT msg, VUINT param1=0, VUINT param2=0);

    inline  UINT getMessageMsg()    const { return msgmsg;    }
    inline VUINT getMessageParam1() const { return msgparam1; }
    inline VUINT getMessageParam2() const { return msgparam2; }
    //end window messaging

  
  protected:
    //this is a section of inheritable events
    virtual HWND onCreateWindow();
    virtual void onInit() {}
    virtual void onPostInit() {}

    //basic win32 message handling functions
    virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);
    
  
  private:
    VUINT wndProc_setup(UINT msg, VUINT param1, VUINT param2);
    static LRESULT CALLBACK static_wndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);


  private:
    vMessageWnd *parent;

    HWND hwnd;
    HWND hparent;

    UINT   msgmsg;
    WPARAM msgparam1;
    LPARAM msgparam2;

    rmxString classname;
};


#endif//_MESSAGEWND_H_
