/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "funcCallback.h"


funcCallback::funcCallback(funcBase *function, HPLUGIN plugin, HLIST item) : cbCore(plugin, item) {
  base = function;
  base->callback = this;
}


funcCallback::~funcCallback() {
  base->callback = NULL;
}


void funcCallback::cb_onSetParam(HPARAM param) {
  if (getRemotePlugin()->param_isNamed(param, "value")) {
    base->onActivated(getRemotePlugin(), getRemoteConfig(), NULL);
  }  else if (getRemotePlugin()->param_isNamed(param, "plugin")) {
    delete this;  //TODO:  implement a better way to "zero" value of funcBase
  }
}


void funcCallback::cb_onSetEnabled(BOOL isenable) {
  if (!isenable) delete this;
}


void funcCallback::cb_onDelete(HLIST item) {
  HPARAM param = getRemotePlugin()->list_getParamHandle(getRemoteConfig(), "value");
  if (param) getRemotePlugin()->param_setValueNumber(param, 0);
  base->onActivated(getRemotePlugin(), getRemoteConfig(), NULL);
  cbCore::cb_onDelete(item);
}
