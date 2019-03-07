/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_STATUS_H_
#define _WND_STATUS_H_


#include "wndSubclass.h"


enum vStatusSide {
  STATUS_LEFT,
  STATUS_RIGHT,
};


class wndStatus : public wndSubclass {
  public:
    wndStatus(wndBase *parent=NULL);
    virtual ~wndStatus();

  public:
    void autoposition();

    void setStatusParts(int *array, int numparts);
    void setStatusText(const char *text, int bar=0);

    char *getStatusText(char *buffer, int bar=0);
    VINT getStatusTextLength(int bar);

    void getStatusRect(RECT *r, int bar);

  protected:
//    virtual UINT onCommand(USHORT id, USHORT cmd, LPARAM lParam);
};


class vButtonStatusWnd : public wndStatus {
  public:
    vButtonStatusWnd(wndBase *parent=NULL);
    virtual ~vButtonStatusWnd();

    void setCaption(const char *cap);
    const char *getCaption() { return caption; }

    wndBase *addButton(const char *text, int id, vStatusSide side=STATUS_LEFT);
    wndBase *getButton(int id);
    void removeButton(int id);

  protected:
    virtual void onResize(int width, int height);

  private:
    rmxList<wndBase*> leftButtons;
    rmxList<wndBase*> rightButtons;
    rmxString caption;
};


#endif //_WND_STATUS_H_
