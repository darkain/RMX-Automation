/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CONFIGPANEITEMS_H_
#define _CONFIGPANEITEMS_H_


#include "../../../sdk/wnd/wndForestItems.h"
#include "../../../sdk/whk/whkColor.h"
#include "ConfigPaneItem.h"
#include <stdlib.h>
#include <Commdlg.h>


class vConfigWnd;
class vConfigPaneWnd;


//---------------------------------------------------------------------------------------------------------


class vConfigPaneLine : public vForestLine, public vConfigPaneItem  {
  public:
    vConfigPaneLine(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert=NULL);
    virtual ~vConfigPaneLine() {}
    virtual void onInit() { vForestLine::onInit();  vConfigPaneItem::onInit(); }
};


//---------------------------------------------------------------------------------------------------------


class vConfigPaneNull : public wndForestItem, public vConfigPaneItem  {
  public:
    vConfigPaneNull(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert=NULL);
    virtual ~vConfigPaneNull() {}

  protected:
    virtual void onInit() { wndForestItem::onInit();  vConfigPaneItem::onInit(); }
//    virtual void cb_onSetParam(HPARAM param);
    virtual void onSetExpanded() { wndForestItem::onSetExpanded(); vConfigPaneItem::setExpanded( wndForestItem::isExpanded() ); }
	virtual void cb_cfgMessage(cbMessage message, HPARAM param, HPARAM insert=NULL);
    void updateAlign();
};


//---------------------------------------------------------------------------------------------------------


class vConfigPaneLink : public vConfigPaneNull {
  public:
    vConfigPaneLink(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert=NULL);
    virtual ~vConfigPaneLink() {}

    virtual void onLeftDown(int x, int y);
};


//---------------------------------------------------------------------------------------------------------


class vConfigPaneButton : public vForestButton, public vConfigPaneItem  {
  public:
    vConfigPaneButton(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert=NULL);
    virtual ~vConfigPaneButton() {}

  protected:
    virtual void onInit() { vForestButton::onInit();  vConfigPaneItem::onInit(); }
    virtual void onSetExpanded() { vForestButton::onSetExpanded();  vConfigPaneItem::setExpanded( wndForestItem::isExpanded() ); }
    virtual void cb_onSetParam(HPARAM param);
    virtual void onButtonClick() { if (isPostInited()) message(GENERIC_CLICK); }
};


//---------------------------------------------------------------------------------------------------------


class vConfigPaneBool : public vForestBool, public vConfigPaneItem  {
  public:
    vConfigPaneBool(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert=NULL);
    virtual ~vConfigPaneBool() {}

  protected:
    virtual void onInit();
    virtual void onSetExpanded() { vForestBool::onSetExpanded(); vConfigPaneItem::setExpanded( wndForestItem::isExpanded() ); }
    virtual void onSetChecked(BOOL checked) { if (isPostInited()) setUserDataInt(checked); }
    virtual void cb_onSetUserData(HPARAM userdata);
};


//---------------------------------------------------------------------------------------------------------


class vConfigPaneSlider : public vForestSlider, public vConfigPaneItem  {
  public:
    vConfigPaneSlider(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert=NULL);
    virtual ~vConfigPaneSlider() {}

  protected:
    virtual void onInit();
    virtual void onTrackWnd(int pos);
    virtual void onSetExpanded() { vForestSlider::onSetExpanded(); vConfigPaneItem::setExpanded( wndForestItem::isExpanded() ); }
    virtual void cb_onSetParam(   HPARAM param   );
    virtual void cb_onSetUserData(HPARAM userdata);
};


//---------------------------------------------------------------------------------------------------------


class vConfigPaneEdit : public vForestEdit, public vConfigPaneItem  {
  public:
    vConfigPaneEdit(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert=NULL);
    virtual ~vConfigPaneEdit() { update(); }

  protected:
    virtual void onInit() { vForestEdit::onInit();  vConfigPaneItem::onInit(); }
    virtual void update() { setUserData/*String*/(getEditText()); }  //todo:  find a way to make this work
    virtual void onSetExpanded() { wndForestItem::onSetExpanded(); vConfigPaneItem::setExpanded( wndForestItem::isExpanded() ); }
    virtual void onEnter() { wndForestItem::onEnter(); update(); }
    virtual void onSetFocus()  { wndForestItem::onSetFocus();            message(GENERIC_GAIN_FOCUS); }
    virtual void onLoseFocus() { wndForestItem::onLoseFocus(); update(); message(GENERIC_LOST_FOCUS); }

	virtual void cb_cfgMessage(cbMessage message, HPARAM param, HPARAM insert=NULL) {
		vConfigPaneItem::cb_cfgMessage(message, param, insert);

		if (message == CB_PARAM_UPDATED  &&  isParamUserData(param)) {
			setEditText(getRemotePlugin()->param_getValue(param));
		}
	}
};


//---------------------------------------------------------------------------------------------------------


class vConfigPaneEditInt : public vConfigPaneEdit {
  public:
    vConfigPaneEditInt(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert=NULL)
      : vConfigPaneEdit(panewnd, cb_plugin, cb_list, parent, insert) {}
    virtual ~vConfigPaneEditInt() { update(); }

  protected:
    virtual void update() { setUserDataInt(atoi(getEditText())); }
};


//---------------------------------------------------------------------------------------------------------


class vConfigPagePathPicker : public wndForestItem, public vConfigPaneItem {
  public:
    vConfigPagePathPicker(vConfigPaneWnd *panewnd, HPLUGIN cb_plugin, HLIST cb_list, wndForestBase *parent, wndForestBase *insert=NULL);
    virtual ~vConfigPagePathPicker() {}

    virtual void onInit();
    virtual void onResize(int width, int height);

    virtual BOOL onCommand(whkInfo *info);

    virtual void cb_onSetUserData(HPARAM userdata);

  private:
    wndBase *nullwnd;
    wndBase *button;
    wndEdit *edit;
};


class vNullColorWnd : public wndBase, public whkColor {
  public:
    vNullColorWnd(const char *name, wndBase *parent) : wndBase(name, parent) {
      whkColor::insertHookWnd(this);
    }

    virtual ~vNullColorWnd() {}
};


#endif //_CONFIGPANEITEMS_H_
