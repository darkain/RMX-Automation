/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "Main.h"
#include "ammo.h"
#include "../../sdk/handles/hplugin.h"


static vAti *ati = new vAti();


vAti::vAti() : plugBase("ATI Remote Wonder") {
  wnd  = NULL;
  tree = NULL;
}


vAti::~vAti() {
  vdelnull(wnd);
}


void vAti::onInit() {
  DEBUGMSG(getName(), "Starting");

  tree = new cfgTree(getName());

  new vAtiEvent("Play",         tree, RMCTRL_PLAY,       "play");
  new vAtiEvent("Pause",        tree, RMCTRL_PAUSE,      "pause");
  new vAtiEvent("Stop",         tree, RMCTRL_STOP,       "stop");
  new vAtiEvent("Stop Watch",   tree, RMCTRL_STOPWATCH);
  new vAtiEvent("Fast Forward", tree, RMCTRL_FF,         "forward");
  new vAtiEvent("Fast Rewind",  tree, RMCTRL_RW,         "backward");
  new vAtiEvent("Volume Up",    tree, RMCTRL_VOLUMEUP,   "volup");
  new vAtiEvent("Volume Down",  tree, RMCTRL_VOLUMEDOWN, "voldown");
  new vAtiEvent("Volume Mute",  tree, RMCTRL_MUTE,       "volmute");
  new vAtiEvent("Record",       tree, RMCTRL_RECORD,     "record");

  //TODO:  see if mouse gets special hints
  new vAtiEvent("Mouse Left Click",         tree, RMCTRL_LEFTMOUSE,             "action0");
  new vAtiEvent("Mouse Right Click",        tree, RMCTRL_RIGHTMOUSE,            "action1");
  new vAtiEvent("Mouse Left Double-Click",  tree, RMCTRL_LEFTMOUSEDOUBLECLICK,  "action2");
  new vAtiEvent("Mouse Right Double-Click", tree, RMCTRL_RIGHTMOUSEDOUBLECLICK, "action3");
  new vAtiEvent("Mouse Up",                 tree, RMCTRL_DIRPADUP,              "up");
  new vAtiEvent("Mouse Down",               tree, RMCTRL_DIRPADDOWN,            "down");
  new vAtiEvent("Mouse Left",               tree, RMCTRL_DIRPADLEFT,            "left");
  new vAtiEvent("Mouse Right",              tree, RMCTRL_DIRPADRIGHT,           "right");
  new vAtiEvent("Mouse Up-Left",            tree, RMCTRL_DIRPADLEFTUP,          "upleft");
  new vAtiEvent("Mouse Up-Right",           tree, RMCTRL_DIRPADRIGHTUP,         "upright");
  new vAtiEvent("Mouse Down-Left",          tree, RMCTRL_DIRPADLEFTDOWN,        "downleft");
  new vAtiEvent("Mouse Down-Right",         tree, RMCTRL_DIRPADRIGHTDOWN,       "downright");
  new vAtiEvent("Mouse Hand",               tree, RMCTRL_HAND);

  new vAtiEvent("Arrow Up",     tree, RMCTRL_UP,          "up");
  new vAtiEvent("Arrow Down",   tree, RMCTRL_DOWN,        "down");
  new vAtiEvent("Arrow Left",   tree, RMCTRL_LEFT,        "left");
  new vAtiEvent("Arrow Right",  tree, RMCTRL_RIGHT,       "right");
  new vAtiEvent("Channel Up",   tree, RMCTRL_CHANNELUP);
  new vAtiEvent("Channel Down", tree, RMCTRL_CHANNELDOWN);
  new vAtiEvent("Resize",       tree, RMCTRL_RESIZE);
  new vAtiEvent("Web Launch",   tree, RMCTRL_WEB_LAUNCH);
  new vAtiEvent("Menu",         tree, RMCTRL_MENU,        "menu");
  new vAtiEvent("Setup",        tree, RMCTRL_SETUP,       "setup");
  new vAtiEvent("Enter",        tree, RMCTRL_ENTER,       "action0");

  for (int i=0; i<16; i++) {
    char str1[16];
    VPRINTF(str1, sizeof(str1), "Button %01X", i);

    char str2[16];
    VPRINTF(str2, sizeof(str2), "button%d", i);

    evntBase *event = new vAtiEvent( str1, tree, ((i>9)?(RMCTRL_A-10+i):(RMCTRL_0+i)), str2 );
  }

  wnd = new vRemoteWonderWnd();
  wnd->init();

  DEBUGMSG(getName(), "Started Sucessfully");
}


void vAti::rwCommand(int command, BOOL state) {
  cfgBase *child = tree->getFirstChild();
  while (child) {
    if (child->getParamInt("cmd") == command) {
      evntPress *event = static_cast<evntSingle*>(child);
      event->setPress(state);
      return;
    }
    child = tree->getNextChild(child);
  }
}


void vAti::onQuit() {
  vdelnull(wnd);
}


//--------------------------------------------------------------------------------------


vRemoteWonderWnd::vRemoteWonderWnd() : vMessageWnd(_CLASS_NAME) {
  setName(getClassName());
}


vRemoteWonderWnd::~vRemoteWonderWnd() {
}


VUINT vRemoteWonderWnd::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == WM_USER+327) {
    HPLUGIN plugin = vHPLUGIN::getMaster();
    plugin->list_message(plugin->list_getRoot(), GENERIC_OPEN, (RMXLONG)ati->getTree()->getListHandle());
  }

  if (msg == WM_USER+328) {
    ati->rwCommand(param1, !!param2);
  }

  return vMessageWnd::wndProc(msg, param1, param2);
}
