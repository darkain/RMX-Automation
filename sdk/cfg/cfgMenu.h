/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_MENU_H_
#define _CFG_MENU_H_


#include "cfgBase.h"


class cfgMenu : public cfgBase {
  public:
    cfgMenu(const char *initname, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~cfgMenu();

  public:
    inline BOOL isChecked() const { return !!getParamInt("checked"); }
    inline void setChecked(BOOL checked) { setParamInt("checked", !!checked); }
    inline void toggle() { setChecked(!isChecked()); }

  protected:
    virtual void onClick(HPLUGIN source) {}
    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2);
};


class cfgMenuSep : public cfgMenu {
  public:
    cfgMenuSep(cfgBase *parent=NULL, cfgBase *insert=NULL);
    virtual ~cfgMenuSep() {}
};


#endif //_CFG_MENU_H_
