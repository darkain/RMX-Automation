/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _UI_KEYBOARD_EVENT_H_
#define _UI_KEYBOARD_EVENT_H_


#include "../../sdk/evnt/evntPress.h"
#include "../../sdk/cfg/config.h"


class uiKeyboard;


class uiKeyboardEvent : public evntPress {
  public:
    uiKeyboardEvent(const char *name, cfgBase *parent, HPARAMLIST paramlist);
    virtual ~uiKeyboardEvent();

  public:
    void setText(const char *text, const char *raw);
    const char *getRawText() const;

  private:
//    uiKeyboard *par;
    cfgBase *line;
    cfgText *cfgtext;
    cfgText *cfgraw;
};


//--------------------------------------------------------------------


class uiKeyboardEventText : public cfgText {
  public:
    uiKeyboardEventText(const char *name, uiKeyboardEvent *par);
    virtual ~uiKeyboardEventText();

  protected:
    virtual void onGainFocus(HPLUGIN source);
    virtual void onLostFocus(HPLUGIN source);

  private:
    uiKeyboardEvent *parent;
};


#endif //_UI_KEYBOARD_EVENT_H_
