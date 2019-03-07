/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _UI_MOUSE_WHEEL_H_
#define _UI_MOUSE_WHEEL_H_


#include "../../sdk/func/funcRelative.h"


class uiMouseWheel : public funcRelative {
  public:
    uiMouseWheel(int iwheel, const char *function_name, const char *script_name, funcGroup *parent);
    ~uiMouseWheel();

  protected:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, double delta);

  private:
    int wheel;
};


#endif  //_UI_MOUSE_WHEEL_H_