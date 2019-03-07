/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _UI_BASE_H_
#define _UI_BASE_H_


#include "../../sdk/plugBase.h"


class uiKeyboard;
class uiMouse;


class uiBase : public plugBase {
  public:
    uiBase();
    virtual ~uiBase();

  protected:
    virtual void onInit();
    virtual void onQuit();

  private:
    uiKeyboard *kb;
    uiMouse    *ms;
};


#endif //_UI_BASE_H_
