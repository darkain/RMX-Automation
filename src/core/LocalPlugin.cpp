/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "LocalPlugin.H"


vLocalPlugin::vLocalPlugin(const char *name, const char *path) : vPlugin(path), cfgRoot(name) {
  setEntry(_RMX_ENTRY);
}


vLocalPlugin::~vLocalPlugin() {
}
