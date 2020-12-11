/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "AppCommand.h"


#ifndef MAKE_APPCOMMAND_LPARAM
#  define MAKE_APPCOMMAND_LPARAM(command, device, keys) \
     MAKELONG(keys, (command & ~FAPPCOMMAND_MASK) | (device & FAPPCOMMAND_MASK))
#endif //MAKE_APPCOMMAND_LPARAM


static vAppCommand *appcmd = new vAppCommand();


LRESULT CALLBACK CmdProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode==HSHELL_APPCOMMAND  &&  appcmd->getWindow()) {
    return appcmd->getWindow()->cmdProc(nCode, wParam, lParam);
  }
  return CallNextHookEx(appcmd->getHook(), nCode, wParam, lParam);
}


vAppCommand::vAppCommand() : plugBase("App Commands") {
  wnd  = NULL;
  tree = NULL;
  hook = NULL;
  unlock();
}


vAppCommand::~vAppCommand() {
  lock();

  if (hook) {
    SetLastError(0);
    UnhookWindowsHookEx(hook);
    if (GetLastError()) {
      char str[128] = "";
      VPRINTF(str, sizeof(str), "ERROR CODE: %d : %d", GetLastError(), hook);
      CONSOLE_ACTIVE->printError(getName(), str);
    }
    hook = NULL;
  }

  vdelnull(wnd);
}


void vAppCommand::onInit() {
  //functions
  funcGroup *funcappcmd = new funcGroup("App Commands", "appcmd", this);

  funcGroup *media = new funcGroup("Media", "media", funcappcmd);
  new vAppFunction("Play/Pause",     "playpause", VK_MEDIA_PLAY_PAUSE, media);
  new vAppFunction("Stop",           "stop",      VK_MEDIA_STOP,       media);
  new vAppFunction("Previous Track", "prev",      VK_MEDIA_PREV_TRACK, media);
  new vAppFunction("Next Track",     "next",      VK_MEDIA_NEXT_TRACK, media);

  funcGroup *volume = new funcGroup("Volume", "volume", funcappcmd);
  new vAppFunction("Up",   "up",   VK_VOLUME_UP,   volume);
  new vAppFunction("Down", "down", VK_VOLUME_DOWN, volume);
  new vAppFunction("Mute", "mute", VK_VOLUME_MUTE, volume);

  funcGroup *browser = new funcGroup("Browser", "browser", funcappcmd);
  new vAppFunction("Home",       "home",     VK_BROWSER_HOME,       browser);
  new vAppFunction("Backwards",  "backward", VK_BROWSER_BACK,       browser);
  new vAppFunction("Forwards",   "forward",  VK_BROWSER_FORWARD,    browser);
  new vAppFunction("Refresh",    "refresh",  VK_BROWSER_REFRESH,    browser);
  new vAppFunction("Stop",       "stop",     VK_BROWSER_STOP,       browser);
  new vAppFunction("Search",     "search",   VK_BROWSER_SEARCH,     browser);
  new vAppFunction("Favourites", "fav",      VK_BROWSER_FAVORITES,  browser);

  funcGroup *launch = new funcGroup("Launch", "launch", funcappcmd);
  new vAppFunction("Mail",          "mail",  VK_LAUNCH_MAIL,         launch);
  new vAppFunction("Media",         "media", VK_LAUNCH_MEDIA_SELECT, launch);
  new vAppFunction("Application 1", "app1",  VK_LAUNCH_APP1,         launch);
  new vAppFunction("Application 2", "app2",  VK_LAUNCH_APP2,         launch);


  //events
  tree = new cfgTree(getName(), this);
  new vAppEvent("Play",           tree, APPCOMMAND_MEDIA_PLAY);
  new vAppEvent("Play/Pause",     tree, APPCOMMAND_MEDIA_PLAY_PAUSE);
  new vAppEvent("Pause",          tree, APPCOMMAND_MEDIA_PAUSE);
  new vAppEvent("Stop",           tree, APPCOMMAND_MEDIA_STOP);
  new vAppEvent("Record",         tree, APPCOMMAND_MEDIA_RECORD);
  new vAppEvent("Previous Track", tree, APPCOMMAND_MEDIA_PREVIOUSTRACK);
  new vAppEvent("Next Track",     tree, APPCOMMAND_MEDIA_NEXTTRACK);
  new vAppEvent("Channel Up",     tree, APPCOMMAND_MEDIA_CHANNEL_UP);
  new vAppEvent("Channel Down",   tree, APPCOMMAND_MEDIA_CHANNEL_DOWN);
  new vAppEvent("Fast Rewind",    tree, APPCOMMAND_MEDIA_REWIND);
  new vAppEvent("Fast Forward",   tree, APPCOMMAND_MEDIA_FAST_FORWARD);

  new vAppEvent("Volume Up",   tree, APPCOMMAND_VOLUME_UP);
  new vAppEvent("Volume Down", tree, APPCOMMAND_VOLUME_DOWN);
  new vAppEvent("Volume Mute", tree, APPCOMMAND_VOLUME_MUTE);
  new vAppEvent("Treble Up",   tree, APPCOMMAND_TREBLE_UP);
  new vAppEvent("Treble Down", tree, APPCOMMAND_TREBLE_DOWN);
  new vAppEvent("Bass Boost",  tree, APPCOMMAND_BASS_BOOST);
  new vAppEvent("Bass Up",     tree, APPCOMMAND_BASS_UP);
  new vAppEvent("Bass Down",   tree, APPCOMMAND_BASS_DOWN);

  new vAppEvent("MIC On/Off Toggle", tree, APPCOMMAND_MIC_ON_OFF_TOGGLE);
  new vAppEvent("MIC Volume Up",     tree, APPCOMMAND_MICROPHONE_VOLUME_UP);
  new vAppEvent("MIC Volume Down",   tree, APPCOMMAND_MICROPHONE_VOLUME_DOWN);
  new vAppEvent("MIC Volume Mute",   tree, APPCOMMAND_MICROPHONE_VOLUME_MUTE);

  new vAppEvent("Browser Backwards", tree, APPCOMMAND_BROWSER_BACKWARD);
  new vAppEvent("Browser Forwards",  tree, APPCOMMAND_BROWSER_FORWARD);
  new vAppEvent("Browser Stop",      tree, APPCOMMAND_BROWSER_STOP);
  new vAppEvent("Browser Home",      tree, APPCOMMAND_BROWSER_HOME);
  new vAppEvent("Browser Refresh",   tree, APPCOMMAND_BROWSER_REFRESH);
  new vAppEvent("Browser Search",    tree, APPCOMMAND_BROWSER_SEARCH);
  new vAppEvent("Browser Favorites", tree, APPCOMMAND_BROWSER_FAVORITES);

  new vAppEvent("Cut",   tree, APPCOMMAND_CUT);
  new vAppEvent("Copy",  tree, APPCOMMAND_COPY);
  new vAppEvent("Paste", tree, APPCOMMAND_PASTE);

  new vAppEvent("Launch App 1", tree, APPCOMMAND_LAUNCH_APP1);
  new vAppEvent("Launch App 2", tree, APPCOMMAND_LAUNCH_APP2);
  new vAppEvent("Launch Mail",  tree, APPCOMMAND_LAUNCH_MAIL);
  new vAppEvent("Close",        tree, APPCOMMAND_CLOSE);

  new vAppEvent("Dictate/Command Control Toggle", tree, APPCOMMAND_DICTATE_OR_COMMAND_CONTROL_TOGGLE);
  new vAppEvent("Correction List",                tree, APPCOMMAND_CORRECTION_LIST);
  new vAppEvent("Spell Check",                    tree, APPCOMMAND_SPELL_CHECK);

  new vAppEvent("New File",   tree, APPCOMMAND_NEW);
  new vAppEvent("Open File",  tree, APPCOMMAND_OPEN);
  new vAppEvent("Save File",  tree, APPCOMMAND_SAVE);
  new vAppEvent("Print File", tree, APPCOMMAND_PRINT);
  new vAppEvent("Undo",       tree, APPCOMMAND_UNDO);
  new vAppEvent("Redo",       tree, APPCOMMAND_REDO);
  new vAppEvent("Find",       tree, APPCOMMAND_FIND);
  new vAppEvent("Help",       tree, APPCOMMAND_HELP);

  new vAppEvent("Send Mail",     tree, APPCOMMAND_SEND_MAIL);
  new vAppEvent("Forward Mail",  tree, APPCOMMAND_FORWARD_MAIL);
  new vAppEvent("Reply to Mail", tree, APPCOMMAND_REPLY_TO_MAIL);

  wnd = new vAppCmdWnd();
  wnd->init();

  //todo: http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/winui/windowsuserinterface/windowing/hooks/usinghooks.asp
//  hook = SetWindowsHookEx(WH_SHELL, CmdProc, vWin32::getInstance(), 0);
//  if (!hook) CONSOLE_ACTIVE->printError(getName(), "Error Registering System Hook");

#if defined _DEBUG || defined _BETA
  char str[32] = "";
  VPRINTF(str, sizeof(str), "HOOK ID: %d", hook);
  CONSOLE_ACTIVE->printWarning(getName(), str);
#endif
}


void vAppCommand::onQuit() {
  lock();

  if (hook) {
    SetLastError(0);
    UnhookWindowsHookEx(hook);
    if (GetLastError()) {
      char str[128] = "";
      VPRINTF(str, sizeof(str), "ERROR CODE: %d : %d", GetLastError(), hook);
      CONSOLE_ACTIVE->printError(getName(), str);
    }
    hook = NULL;
  }
  
  vdelnull(wnd);
}


evntSingle *vAppCommand::getEvent(int cmd) {
  evntSingle *event = cmds.getFirstItem();
  while (event) {
    if (event->getParamInt("cmd") == cmd) return event;
    event = cmds.getNextItem(event);
  }
  return NULL;
}


void vAppCommand::event(short ID) {
  evntSingle *event = getEvent(ID);
  if (event) {
    lock();
    event->activate();
    unlock();
  }
}


//---------------------------------------------------------------------------------------------------


vAppCmdWnd::vAppCmdWnd() : vTimer(16), wndBase("AppCommand") {
  wait = FALSE;

  setParentHWnd(NULL);  //by forcing a NULL parent, it wont create a message only window, allowing us to recieve callbacks from the OS
  setWindowStyle(WS_POPUP | WS_VISIBLE);
  setWindowStyleEx(WS_EX_TOOLWINDOW);

  moveresize(-1000, -1000, 0, 0);
}


vAppCmdWnd::~vAppCmdWnd() {
}


UINT vAppCmdWnd::cmdProc(int code, UINT param1, UINT param2) {
#ifdef _DEBUG
  char str[256];
  VPRINTF(str, sizeof(str), "Get ID-A: %04x", GET_APPCOMMAND_LPARAM(param2));
  CONSOLE_ACTIVE->printWarning(appcmd->getName(), str);
#endif

  if (!wait  &&  appcmd->isUnlocked()) {
#ifdef _DEBUG
    CONSOLE_ACTIVE->printWarning(appcmd->getName(), "processing");
#endif
    wait = TRUE;
    appcmd->event(GET_APPCOMMAND_LPARAM(param2));
    startTimer();
    return TRUE;
  }

  return 0;
}


VUINT vAppCmdWnd::wndProc(UINT msg, VUINT param1, VUINT param2) {
#ifdef _DEBUG
  if (msg == WM_APPCOMMAND) {
    char str[256];
    VPRINTF(str, sizeof(str), "[AppCommand] Get ID-B: %04x", GET_APPCOMMAND_LPARAM(param2));
  CONSOLE_ACTIVE->printWarning(appcmd->getName(), str);
  }
#endif

  if (!wait  &&  appcmd->isUnlocked()) switch (msg) {
    case WM_APPCOMMAND:
#ifdef _DEBUG
      CONSOLE_ACTIVE->printWarning(appcmd->getName(), "processing");
#endif
      wait = TRUE;
      appcmd->event(GET_APPCOMMAND_LPARAM(param2));
      startTimer();
      return TRUE;
  }

  return vMessageWnd::wndProc(msg, param1, param2);
}


//---------------------------------------------------------------------------------------------------


vAppEvent::vAppEvent(const char *name, cfgBase *par, int cmd) : evntSingle(name, par) {
  insertParamIntNoSave("cmd", cmd);
  appcmd->addCommand(this);
}


//---------------------------------------------------------------------------------------------------


vAppFunction::vAppFunction(const char *function_name, const char *script_name, int command, funcGroup *parent)
 : funcSingle(function_name, script_name, parent), vTimer(100), cmd(command) {
}


vAppFunction::~vAppFunction() {
}


void vAppFunction::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  BOOL lock = appcmd->isLocked();
  if (!lock) appcmd->lock();

#ifdef _DEBUG
  CONSOLE_ACTIVE->printWarning(appcmd->getName(), "locking");
#endif

  int scan = MapVirtualKey(cmd, 0);
  keybd_event(cmd, scan, KEYEVENTF_EXTENDEDKEY, 0);
  Sleep(1);

  if (!lock) startTimer();
}


void vAppFunction::onTimer(DWORD systemTime) {
  stopTimer();

#ifdef _DEBUG
  CONSOLE_ACTIVE->printWarning(appcmd->getName(), "unlocking");
#endif

  appcmd->unlock();
}
