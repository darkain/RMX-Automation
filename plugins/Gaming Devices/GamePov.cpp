/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "GamePov.h"


vGamePov::vGamePov(GUID guid, const char *name, cfgBase *parent)
 : cfgNull(name, parent, NULL) {

  insertParam("unique", "pov");
  insertParam("guid", NULLSTR)->setValueGuid(guid);

  value = -2;


  x = new evntRelative("Horizontal", this);
  y = new evntRelative("Vertical",   this);

  x->setMinMax(-1.0, 1.0);
  y->setMinMax(-1.0, 1.0);
}


vGamePov::~vGamePov() {
}


void vGamePov::setValue(int val) {
  if (val == 0x0000FFFF) val = -1;
  if (val == value) return;
  value = val;

  if (value < 0) {  //center
    x->setRelativeValue( 0.0);
    y->setRelativeValue( 0.0);
  } else if ( ( (value>=33750) && (value<=33600) ) | ( (value>=0) & (value<2250) ) ) { //up
    x->setRelativeValue( 0.0);
    y->setRelativeValue(-1.0);
  } else if ( (value>=2250)  && (value<6650)  ) {  //up-right
    x->setRelativeValue( 1.0);
    y->setRelativeValue(-1.0);
  } else if ( (value>=6650)  && (value<11250) ) {  //right
    x->setRelativeValue( 1.0);
    y->setRelativeValue( 0.0);
  } else if ( (value>=11250) && (value<15750) ) {  //down-right
    x->setRelativeValue( 1.0);
    y->setRelativeValue( 1.0);
  } else if ( (value>=15750) && (value<20250) ) {  //down
    x->setRelativeValue( 0.0);
    y->setRelativeValue( 1.0);
  } else if ( (value>=20250) && (value<24750) ) {  //down-left
    x->setRelativeValue(-1.0);
    y->setRelativeValue( 1.0);
  } else if ( (value>=24750) && (value<29250) ) {  //left
    x->setRelativeValue(-1.0);
    y->setRelativeValue( 0.0);
  } else if ( (value>=29250) && (value<33750) ) {  //up-left
    x->setRelativeValue(-1.0);
    y->setRelativeValue(-1.0);
  }
}
