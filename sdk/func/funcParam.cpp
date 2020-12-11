/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "funcParam.h"


funcParam::funcParam(const char *itemName, const char *flagName, const char *type, cfgBase *parent, cfgBase *insert, HPARAMLIST paramlist)
 : cfgBase(itemName, type, parent, insert, paramlist) {

  setSavable(FALSE);
  setParam("paramname", flagName);
  setParam("prefix",    NULLSTR);
  setParam("suffix",    NULLSTR);
  setParam("default",   NULLSTR);
}


funcParam::~funcParam() {
}


cfgBase *funcParam::addListItem(const char *name) {
  cfgBase *list = new cfgBase(name, "ListItem", this);
  return list;
}


void funcParam::setDefaultValue(cfgBase *list) {
  if (list) setDefaultValue(list->getName());
  else setDefaultValue("");
}
