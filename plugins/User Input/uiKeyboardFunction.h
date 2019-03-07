/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _UI_KEYBOARD_FUNCTION_H_
#define _UI_KEYBOARD_FUNCTION_H_


#include "../../sdk/func/funcPress.h"


class uiKeyboardFunction : public funcPress {
  public:
    uiKeyboardFunction();
    virtual ~uiKeyboardFunction();

  protected:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    void emulateKeyStrokes(const char *string, DWORD flags);
    void sendKey(short vkcode,    DWORD flags, DWORD modifiers=0);
    void sendMod(DWORD modifiers, DWORD flags=0);

};


#endif //_UI_KEYBOARD_FUNCTION_H_
