/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "Debris.h"


static plugDebris *debris = new plugDebris();


plugDebris::plugDebris() : plugBase("Debris") {
}


//wparam=2000 - returns number of objects
void plugDebris::onInit() {
  funcGroup *debfunc = new funcGroup("Debris", "debris");
  new debrisCommand("About",            "about",      debfunc,   73);
  new debrisCommand("Controls",         "controls",   debfunc,   27);
  new debrisCommand("Generate Piclist", "piclist",    debfunc, 1776);
  new debrisCommand("Save Image As",    "saveas",     debfunc, 1492);
  new debrisCommand("Fullscreen",       "fullscreen", debfunc,    3);
  new debrisCommand("Minimize",         "minimize",   debfunc,    6);
  new debrisCommand("Restore",          "restore",    debfunc,   10);
  new debrisCommand("Pause",            "pause",      debfunc,    4);
  new debrisCommand("Lower CPU Usage",  "lowcpu",     debfunc,    5);
  new debrisCommand("Clear Canvas",     "clear",      debfunc,    2);
  new debrisCommand("Exit",             "exit",       debfunc,    1);
}


void plugDebris::onQuit() {
}


void plugDebris::command(UINT command) {
  if (command == 0) return;
  HWND hwnd = FindWindow(L"DebrisWnd", NULL);
  if (!hwnd) {
    CONSOLE_ACTIVE->printWarning(getName(), "Debris window not found");
    return;
  }
  PostMessage(hwnd, WM_COMMAND, command, 0);
}


//---------------------------------------------------------------------------


debrisCommand::debrisCommand(const char *function_name, const char *script_name, funcGroup *parent, int debrisid)
 : funcSingle(function_name, script_name, parent) {

  (insertParamIntNoSave("debris", debrisid))->setReadOnly(TRUE);
}


void debrisCommand::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  debris->command(getParamInt("debris"));
}
