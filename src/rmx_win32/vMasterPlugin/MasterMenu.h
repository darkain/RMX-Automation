/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __RMXWIN32_MASTERMENU_H__
#define __RMXWIN32_MASTERMENU_H__


#include "../../../sdk/cfg/cfgMenu.h"
#include "../../../sdk/cb/cbBase.h"


class vMasterMenu : public cfgMenu/*, public cbBase*/ {
  public:
    vMasterMenu(const char *name, cfgBase *parent, cfgBase *watch)
      : cfgMenu(name, parent) /*, cbBase(watch)*/ {
//TODO: FIX THIS SHIT!!
//      setChecked( getConfig()->getUserDataInt() );
		appendItem(watch);
    }

    virtual ~vMasterMenu() {}

  protected:
    virtual void onClick(HPLUGIN source) {
//TODO: FIX THIS SHIT!!
//      getConfig()->setUserDataInt( !getConfig()->getUserDataInt() );
      cfgMenu::onClick(source);
    }

    virtual void cb_onSetUserData(const prmBase *userdata) {
//TODO: FIX THIS SHIT!!
//      setChecked( getConfig()->getUserDataInt() );
//      cbBase::cb_onSetUserData(userdata);
    }
};


#endif //__RMXWIN32_MASTERMENU_H__
