/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../rmxBase.h"
#include "cbCore.h"


cbCore::cbCore(HPLUGIN plugin, HLIST item) {
  plug = plugin;
  list = NULL;
  prm  = NULL;
  setRemoteConfig(item);
}


cbCore::~cbCore() {
  setRemoteConfig(NULL);
}


void cbCore::setRemoteConfig(HLIST item) {
  if (item == list) return;
  
  if (list) plug->list_removeCallback(list, this);
  
  list = item;
  
  if (list) {
	  prm = plug->list_getListParam(list);
	  plug->list_insertCallback(list, this);
  } else {
	  prm = NULL;
  }
}
