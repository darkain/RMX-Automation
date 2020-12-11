/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _FUNC_NONE_H_
#define _FUNC_NONE_H_


#include "../../sdk/func/funcBase.h"


class funcNone : public funcBase {
  public:
    funcNone(cfgRoot *parent) : funcBase("[NONE]", NULL, parent) {}
    virtual ~funcNone() {}

  protected:

    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output) {}
};


#endif //_FUNC_NONE_H_
