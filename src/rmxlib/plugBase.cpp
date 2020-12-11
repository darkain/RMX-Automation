/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#define RMX_LIB


#include "../../sdk/rmxBase.h"
#include "../../sdk/plugBase.h"
#include "rmx_msg.h"


plugBase *plugin = NULL;
extern UINT xmlStatus;
extern vMSG extrmxmsg;



plugBase::plugBase(const char *name) : cfgRoot(name) {
#ifdef _DEBUG
  if (plugin != NULL) { // TODO:  switch this over to ASSERT
    CONSOLE_MAIN->printError(getName(), "SERIOUS FUCKING ERROR: Multiple instances of class plugBase");
  }
#endif
  plugin = this;

  srand(RMXOS::getTick());
/*  //TODO: reimpliment this!!
  cfgBase *id = getID();
  id->setParamInt("lib", _SDK_BUILD);
  id->setParamReadOnly("sdk", TRUE);
  id->setParamReadOnly("lib", TRUE);
*/
}


plugBase::~plugBase() {
  if (!plugin) return;
  removeAllChildren();
  RmxNul( (VMSG::SYSTEM, 2, getInstance()) );
  plugin = NULL;
}


void plugBase::unload(BOOL sessiononly) {
  RmxNul( (1, 3, extrmxmsg.plugin, sessiononly) );
  plugin = NULL;
}


HPLUGIN plugBase::getThisPluginHandle() const {
  return extrmxmsg.plugin;
}


void plugBase::consoleCommand(const char *command) {
  RmxNul( (VMSG::SYSTEM, 301, command) );
}


BOOL plugBase::isLoadingXml() const { return (xmlStatus == 1); }
BOOL plugBase::isSavingXml()  const { return (xmlStatus == 2); }
