/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_BUTTON_MENU_H_
#define _CFG_BUTTON_MENU_H_


#include "cfgButton.h"


class cfgButtonMenu : public cfgButton {
  protected:
    cfgButtonMenu(const char *initname, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~cfgButtonMenu();

  protected:
    virtual void onClick(HPLUGIN source);
};


#endif //_CFG_BUTTON_MENU_H_

