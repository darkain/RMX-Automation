/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _FUNC_RELATIVE_H_
#define _FUNC_RELATIVE_H_


#include "funcBase.h"


class funcRelative : public funcBase {
  protected:
    funcRelative(const char *function_name, const char *script_name, cfgRoot   *parent);
    funcRelative(const char *function_name, const char *script_name, funcGroup *parent=NULL);

  public:
    virtual ~funcRelative();

  protected:
    //if using funcRelative, use "onActivated" version with "double delta" at the end (see below)
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output);
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, double delta)=0;
};


#endif //_FUNC_RELATIVE_H_
