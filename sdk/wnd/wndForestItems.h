/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_FOREST_ITEMS_H_
#define _WND_FOREST_ITEMS_H_


#include "wndForestItem.h"
#include "wndEdit.h"
#include "../whk/whkTrack.h"


//---------------------------------------------------------------------------------------------------------


class vForestLine : public wndForestItem {
  public:
    vForestLine(wndForestBase *parent, wndForestBase *insert=NULL) : wndForestItem(parent) {
      setCollapseHeight(0);
      setWindowStyleEx(WS_EX_DLGMODALFRAME);
    }
    virtual ~vForestLine() {}
};


//---------------------------------------------------------------------------------------------------------


class vForestButton : public wndForestItem {
  public:
    vForestButton(wndForestBase *parent, wndForestBase *insert=NULL);
    virtual ~vForestButton();

    void setDescription(const char *desc);
    const char *getDescription() const;

    void setButtonText(const char *text);
    const char *getButtonText() const;

  protected:
    virtual void onButtonClick() {}

    //call up on all of these
    virtual void onSetItemEnabled(BOOL en);

    virtual void onInit();
    virtual void onResize(int width, int height);
    virtual BOOL onCommand(whkInfo *info);
//    virtual UINT onCommand(USHORT id, USHORT cmd, LPARAM lParam);

    virtual void onSetHighlighted(BOOL highlighted);
    virtual void onSetMouseOver(BOOL over);

  private:
    wndBase  *button;
    wndLabel *label;
};


//---------------------------------------------------------------------------------------------------------


class vForestBool : public wndForestItem {
  public:
    vForestBool(wndForestBase *parent, wndForestBase *insert=NULL);
    virtual ~vForestBool();

    void setChecked(BOOL checked);
    BOOL getChecked() const;

  protected:
    virtual void onSetChecked(BOOL checked) {}

    virtual void onSetItemEnabled(BOOL en);

    virtual void onInit();
    virtual void onResize(int width, int height);
    virtual void onLeftDown(int x, int y);
    virtual BOOL onCommand(whkInfo *info);
//    virtual UINT onCommand(USHORT id, USHORT cmd, LPARAM lParam);

    virtual void onSetHighlighted(BOOL highlighted);
    virtual void onSetMouseOver(BOOL over);

  private:
    wndBase *check;
};


//---------------------------------------------------------------------------------------------------------


class vForestSlider : public wndForestItem, public whkTrack {
  public:
    vForestSlider(wndForestBase *parent, wndForestBase *insert=NULL);
    virtual ~vForestSlider();

  protected:
    virtual void onSetItemEnabled(BOOL en);

    virtual void onInit();
    virtual void onResize(int width, int height);
    virtual void onSetHighlighted(BOOL highlighted);
    virtual void onSetMouseOver(BOOL over);

  private:
    wndBase *slider;
};


//---------------------------------------------------------------------------------------------------------


class vForestEdit : public wndForestItem {
  public:
    vForestEdit(wndForestBase *parent, wndForestBase *insert=NULL);
    virtual ~vForestEdit();

    void setEditText(const char *text);
    const char *getEditText() const;

    wndEdit *getEditWnd() { return edit; }

  protected:
    virtual void onInit();
    virtual void onResize(int width, int height);

    virtual void onSetItemEnabled(BOOL en);

    virtual void onEditEnter() {}
    virtual void onEditSetFocus() {}
    virtual void onEditLoseFocus() {}

  private:
    wndEdit *edit;
};


class vForestEditWnd : public wndEdit {
  public:
    vForestEditWnd(wndForestItem *parent) : wndEdit(parent) {
      appendWindowStyle(ES_RIGHT|ES_AUTOHSCROLL);
    }
    virtual ~vForestEditWnd() {}
    virtual void onEnter()     { getvParent()->onEnter();     }
    virtual void onSetFocus()  { getvParent()->onSetFocus();  }
    virtual void onLoseFocus() { getvParent()->onLoseFocus(); }
};


//---------------------------------------------------------------------------------------------------------


#endif //_WND_FOREST_ITEMS_H_
