/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_BUTTON_H_
#define _CFG_BUTTON_H_


#include "cfgBase.h"


/*
this class does not use UserData at all, this way inherited classes
can safely use UserData for their own needs, such as vFunction
*/

//this is made protected so that way you are forced to inherit... helps
//make sure that things happen correctly.  :)
class cfgButton : public cfgBase {
  protected:
    cfgButton(const char *name, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    cfgButton(const char *name, const char *text, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    cfgButton(const char *name, const char *text, const char *desc, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~cfgButton();

  public:
    inline const char *getButtonText()    const { return getParam("text");  }
    inline void setButtonText(const char *text) { setParam("text", text);   }

    inline const char *getButtonDescription()    const { return getParam("desc"); }
    inline void setButtonDescription(const char *desc) { setParam("desc", desc);  }

  protected:
    virtual void onClick(HPLUGIN source) {}
    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2);
};


#endif //_CFG_BUTTON_H_
