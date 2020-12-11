/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_DROPLIST_H_
#define _WND_DROPLIST_H_


#include "wndSubclass.h"
#include "../whk/whkCommand.h"


//TODO: RENAME THIS
class vDroplistItem;


class wndDroplist : public wndSubclass, public whkCommand {
  friend class vDroplistItem;

  public:
    wndDroplist(wndBase *parent);
    virtual ~wndDroplist();

    vDroplistItem *addDroplistItem(const char *name, void *data=NULL);
    void delDroplistItem(vDroplistItem *item);

    void clearDroplist();

    vDroplistItem *getSelectedItem();


  protected:
    virtual BOOL onCommand(whkInfo *info);

    virtual void onSelectionChange();

    virtual vDroplistItem *newDroplistItem(const char *name, void *data=NULL);

  protected:
    rmxList<vDroplistItem*> listItems;
};


class vDroplistItem : public rmxString {
  friend class wndDroplist;

  public:
    vDroplistItem(const char *name, void *thedata=NULL);
    virtual ~vDroplistItem();

  protected:
    virtual void onSelected()   {}
    virtual void onDeselected() {}

    inline void setWnd(wndDroplist *newwnd) { wnd = newwnd; }
    inline wndDroplist *getWnd() { return wnd; }

  public:
    int getPosition();

    void selectItem();

    inline BOOL getInitialSelect()   const { return initialSelect; }
    inline void setInitialSelect(BOOL sel) { initialSelect =  sel; }
    
  protected:
    void        *data;
    wndDroplist *wnd;
    BOOL         initialSelect;
};


#endif //_WND_DROPLIST_H_
