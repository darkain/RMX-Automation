/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "GameButton.h"


vGameButton::vGameButton(int btnid, const char *name, cfgBase *parent)
 : evntPress(name, parent, NULL) {
  
  insertParam("unique", "button");

  id = btnid;
  pressed = FALSE;

  switch (btnid) {
    case 0: setHint("action0"); break;
    case 1: setHint("action1"); break;
    case 2: setHint("action2"); break;
    case 3: setHint("action3"); break;
    case 4: setHint("action4"); break;
    case 5: setHint("action5"); break;
    case 6: setHint("action6"); break;
    case 7: setHint("action7"); break;
    case 8: setHint("action8"); break;
    case 9: setHint("action9"); break;
  }
}


vGameButton::~vGameButton() {
}


void vGameButton::setPressed(BOOL press) {
  press = !!press;
  if (pressed == press) return;
  pressed = press;

  if (pressed) {
    activatePress();
  } else {
    deactivatePress();
  }
}
