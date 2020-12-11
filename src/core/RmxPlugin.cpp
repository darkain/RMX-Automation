/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"

#include "RmxPlugin.h"

#include "callbacks/cbConsole.h"


RmxPlugin::RmxPlugin(const char *fname, HMODULE hmod, RMX_ENTRY ventry) : vPlugin(fname, hmod) {
  setEntry(ventry);
  status = x_Load();
}


RmxPlugin::~RmxPlugin() {
  if (status != 0) {
    char str[4096];
    VPRINTF(str, sizeof(str), "Unloading Plugin: %s", getFilename());
    CONSOLE_BROADCAST->print("Core", str);
    x_Unload();
  }
}


void RmxPlugin::onFirstLoad() {
  DEBUGMSG("XML", "ON FIRST LOAD");
  RmxMsg(VMSG::CONFIG_BASE, FIRSTLOAD, list_getRoot());
  DEBUGMSG("XML", "ON FIRST LOAD - SUCCESS");
}
/*
void RmxPlugin::onPreSaveSettings() {
  DEBUGMSG("XML", "ON PRE SAVE");
  RmxMsg(VMSG::CONFIG_BASE, PRESAVESETTINGS, list_getRoot());
  RmxMsg(VMSG::SYSTEM, 300, 2);
  DEBUGMSG("XML", "ON PRE SAVE - SUCCESS");
}
*/
/*
void RmxPlugin::onPreLoadSettings() {
  DEBUGMSG("XML", "ON PRE LOAD");
  RmxMsg(VMSG::CONFIG_BASE, PRELOADSETTINGS, list_getRoot());
  RmxMsg(VMSG::SYSTEM, 300, 1);
  DEBUGMSG("XML", "ON PRE LOAD - SUCCESS");
}
*/
/*
void RmxPlugin::onSaveSettings() {
  DEBUGMSG("XML", "ON POST SAVE");
  RmxMsg(VMSG::CONFIG_BASE, SAVESETTINGS, list_getRoot());
  RmxMsg(VMSG::SYSTEM, 300, (int)0);
  DEBUGMSG("XML", "ON POST SAVE - SUCCESS");
}
*/
/*
void RmxPlugin::onLoadSettings() {
  DEBUGMSG("XML", "ON POST LOAD");
  RmxMsg(VMSG::CONFIG_BASE, LOADSETTINGS, list_getRoot());
  RmxMsg(VMSG::SYSTEM, 300, (int)0);
  DEBUGMSG("XML", "ON POST LOAD - SUCCESS");
}
*/

BOOL RmxPlugin::testPlugin() {
  return (x_test() >= sizeof(rmxMsg));
}
