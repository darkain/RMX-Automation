/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#pragma comment(lib, "Winmm.lib")


#include "vpedal.h"
#include <mmsystem.h>


static vPedal *VPEDAL = new vPedal();


vPedal::vPedal() : plugBase("V-Pedal") {
}


void vPedal::onInit() {
  new vPedalTree(getName(), this);
}


//---------------------------------------------------------


vPedalTree::vPedalTree(const char *name, cfgBase *parent) : cfgTree(name, parent), vTimer(50, TRUE) {
  button   = 0;
  play     = new evntPress("Play",     this);
  backward = new evntPress("Backward", this);
  forward  = new evntPress("Forward",  this);

  //TODO:  i think some/all of these are obsoleted
  play->setHint("play");
  backward->setHint("backward");
  forward->setHint("forward");
}


vPedalTree::~vPedalTree() {
}


void vPedalTree::onTimer(DWORD systemTime) {
  JOYINFO info;
  ZeroMemory(&info, sizeof(info));

  if (joyGetPos(0, &info) == JOYERR_NOERROR) {
    int newbutton = info.wButtons & 0x3;

    if (newbutton != button) {
      play->setPress(     newbutton == 0x3 );
      backward->setPress( newbutton == 0x1 );
      forward->setPress(  newbutton == 0x2 );
      button = newbutton;
    }

  } else {
    button = 0x0;
  }
}

