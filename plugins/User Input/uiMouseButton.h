/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _UI_MOUSE_BUTTON_H_
#define _UI_MOUSE_BUTTON_H_


#include "../../sdk/func/funcPress.h"


class uiMouseButton : public funcPress {
  public:
    uiMouseButton(int iup, int idown, int idata, const char *function_name, const char *script_name, funcGroup *parent);
    virtual ~uiMouseButton();

  protected:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    int up;
    int down;
    int data;
};


#endif //_UI_MOUSE_BUTTON_H_
