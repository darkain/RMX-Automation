/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "uiBase.h"
#include "uiKeyboard.h"
#include "uiMouse.h"


static uiBase *UIBASE = new uiBase();


uiBase::uiBase() : plugBase("User Input") {
  kb = NULL;
  ms = NULL;
}


uiBase::~uiBase() {
  vdelnull(ms);
  vdelnull(kb);
}


void uiBase::onInit() {
  kb = new uiKeyboard(this);
  ms = new uiMouse(   this);
}


void uiBase::onQuit() {
  vdelnull(ms);
  vdelnull(kb);
}
