/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _FUNC_SINGLE_H_
#define _FUNC_SINGLE_H_


#include "funcPress.h"


class funcSingle : public funcPress {
  protected:
    funcSingle(const char *function_name, const char *script_name, cfgRoot   *parent);
    funcSingle(const char *function_name, const char *script_name, funcGroup *parent=NULL);

  public:
    virtual ~funcSingle();

  protected:
    //if using funcSingle, use "onActivated" version with "BOOL state" at the end (see below)
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output);
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state)=0;
};


#endif //_FUNC_SINGLE_H_
