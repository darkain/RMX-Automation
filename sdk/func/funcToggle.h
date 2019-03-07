/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _FUNC_TOGGLE_H_
#define _FUNC_TOGGLE_H_


#include "funcPress.h"


class funcToggle : public funcPress {
  protected:
    funcToggle(const char *function_name, const char *script_name, cfgRoot   *parent);
    funcToggle(const char *function_name, const char *script_name, funcGroup *parent=NULL);

  public:
    virtual ~funcToggle();

  public:
    void setCurrentState(BOOL state);

  protected:
    //if using funcToggle, use "onActivated" version with "BOOL state" at the end (see below)
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output);
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state)=0;
};


#endif //_FUNC_TOGGLE_H_
