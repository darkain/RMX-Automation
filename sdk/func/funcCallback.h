/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _FUNC_CALLBACK_H_
#define _FUNC_CALLBACK_H_


#include "funcBase.h"
#include "../cb/cbCore.h"


class funcCallback : public cbCore {
  friend class funcBase;

  private:
    funcCallback(funcBase *function, HPLUGIN plugin, HLIST item);
    virtual ~funcCallback();

  protected:
    virtual void cb_onSetParam(HPARAM param);
    virtual void cb_onSetEnabled(BOOL isenable);


    virtual void cb_onDelete(HLIST item);

  private:
    funcBase *base;
};


#endif //_FUNC_CALLBACK_H_
