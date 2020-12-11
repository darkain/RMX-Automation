/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _GAME_BUTTON_H_
#define _GAME_BUTTON_H_


#include "Gaming.h"


class vGameButton : public evntPress {
  public:
    vGameButton(int btnid, const char *name, cfgBase *parent);

    virtual ~vGameButton();

  public:
    void setPressed(BOOL press);

    inline int getId()       const { return id; }
    inline BOOL getPressed() const { return pressed; }

  private:
    BOOL pressed;
    int id;
};


#endif //_GAME_BUTTON_H_
