/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CFG_TEXT_H_
#define _CFG_TEXT_H_


#include "cfgBase.h"


/*
this class uses UserData to store its displayed text string
*/

class cfgText : public cfgBase {
  public:
    cfgText(const char *initname, cfgBase *parent=NULL, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~cfgText();

  public:
    inline void setText(const char *text) { setValueString(text); }
    inline const char *getText() const { return getValueSafe(); }

  protected:
//    virtual void onSetParam(vConfigParam *param);
    virtual RMXLONG onMessage(HPLUGIN source, RMXLONG message, RMXLONG param1, RMXLONG param2);

    virtual void onGainFocus(HPLUGIN source) {}
    virtual void onLostFocus(HPLUGIN source) {}
};


#endif //_CFG_TEXT_H_
