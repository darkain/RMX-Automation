/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _FUNC_PRESS_H_
#define _FUNC_PRESS_H_


#include "funcBase.h"


class funcPress : public funcBase {
  protected:
    funcPress(const char *function_name, const char *script_name, cfgRoot   *parent);
    funcPress(const char *function_name, const char *script_name, funcGroup *parent=NULL);

  public:
    virtual ~funcPress();

  protected:
    //if using funcPress, use "onActivated" version with "BOOL state" at the end (see below)
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output);
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state)=0;
};


#endif //_FUNC_PRESS_H_
