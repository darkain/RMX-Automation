/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _UI_MOUSE_AXIS_H_
#define _UI_MOUSE_AXIS_H_


#include "../../sdk/func/funcAbsolute.h"


class uiMouseAxis : public funcAbsolute {
  public:
    uiMouseAxis(int iaxis, const char *function_name, const char *script_name, funcGroup *parent);
    ~uiMouseAxis();

  protected:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, double value);

  private:
    int axis;
};


#endif //_UI_MOUSE_AXIS_H_
