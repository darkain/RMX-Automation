/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include <stdio.h>
#include "RemoteCenter.H"
#include <shlobj.h>


static vRemoteCenter *remoteCenter = new vRemoteCenter();


#define CLASSNAME "RMX_RemoteCenter"
#define FILENAME CLASSNAME ".key"


vRemoteCenter::vRemoteCenter() : plugBase("Remote Center") {
  tree       = NULL;
  wnd        = NULL;
  rcMajor    = 0;
  rcMinor    = 0;
  rcRevision = 0;
  rcFound    = FALSE;
}


vRemoteCenter::~vRemoteCenter() {
  vdelnull(wnd);
}


void vRemoteCenter::onInit() {
  tree = new cfgTree(getName(), this);

  new vRemoteEvent("Play",              tree, 6910, "play");
  new vRemoteEvent("Play/Pause",        tree, 6923, "playpause");
  new vRemoteEvent("Pause",             tree, 6913, "pause");
  new vRemoteEvent("Stop",              tree, 6911, "stop");
  new vRemoteEvent("Stop/Eject",        tree, 6924, "stop");
  new vRemoteEvent("Eject",             tree, 6912);
  new vRemoteEvent("Fast Rewind",       tree, 6914, "backward");
  new vRemoteEvent("Fast Forward",      tree, 6915, "forward");
  new vRemoteEvent("Slow Motion",       tree, 6921);
  new vRemoteEvent("Step Forward",      tree, 6922);
  new vRemoteEvent("Previous Track",    tree, 6916, "prevtrack");
  new vRemoteEvent("Next Track",        tree, 6917, "nexttrack");
  new vRemoteEvent("Play Next",         tree, 6925, "nexttrack");
  new vRemoteEvent("Volume Up",         tree, 6940, "volup");
  new vRemoteEvent("Volume Down",       tree, 6941, "voldown");
  new vRemoteEvent("Volume Mute",       tree, 6942, "volmute");

  new vRemoteEvent("Arrow Up",          tree, 6950, "up");
  new vRemoteEvent("Arrow Down",        tree, 6951, "down");
  new vRemoteEvent("Arrow Left",        tree, 6952, "left");
  new vRemoteEvent("Arrow Right",       tree, 6953, "right");
  new vRemoteEvent("Select/OK/Enter",   tree, 6954, "action0");
  new vRemoteEvent("Close/Cancel/Exit", tree, 6955, "action1");

  new vRemoteEvent("Power",             tree, 6995, "closeapp");
  new vRemoteEvent("EAX",               tree, 6943);
  new vRemoteEvent("Options",           tree, 6931, "setup");
  new vRemoteEvent("Display",           tree, 6932, "menu");
  new vRemoteEvent("Return",            tree, 6956, "action1");
  new vRemoteEvent("Start",             tree, 6930, "action0");
  new vRemoteEvent("Clear",             tree, 6957);
  new vRemoteEvent("CMSS",              tree, 6945);
  new vRemoteEvent("Record",            tree, 6926, "record");
  new vRemoteEvent("Function",          tree, 6960);
  new vRemoteEvent("Left/RW",           tree, 6958, "backward");
  new vRemoteEvent("Right/FF",          tree, 6959, "forward");

  new vRemoteEvent("Button 0",          tree, 6900, "button0");
  new vRemoteEvent("Button 1",          tree, 6901, "button1");
  new vRemoteEvent("Button 2",          tree, 6902, "button2");
  new vRemoteEvent("Button 3",          tree, 6903, "button3");
  new vRemoteEvent("Button 4",          tree, 6904, "button4");
  new vRemoteEvent("Button 5",          tree, 6905, "button5");
  new vRemoteEvent("Button 6",          tree, 6906, "button6");
  new vRemoteEvent("Button 7",          tree, 6907, "button7");
  new vRemoteEvent("Button 8",          tree, 6908, "button8");
  new vRemoteEvent("Button 9",          tree, 6909, "button9");

  if (BuildConfig()) {
    wnd = new vRemoteCenterWnd();
    wnd->init();
  }
}


void vRemoteCenter::onQuit() {
  vdelnull(wnd);
}


//-----------------------------------------------------------------------------------------------


bool vRemoteCenter::GetPath(char *Path) {
  const char RCRegString[] = "Software\\Creative Tech\\Creative RemoteCenter\\RcMan";
  const char MCRegString[] = "SOFTWARE\\Creative Tech\\Creative MediaSource\\1.0";
  const char RMRegString[] = "SOFTWARE\\Creative Tech\\Software Installed\\MediaSource\\RemoteControl";

  char str[256] = "";

  bool ret = regKeyString(HKEY_LOCAL_MACHINE, RCRegString, "Path", Path);
  if (ret) {
    if (regKeyString(HKEY_LOCAL_MACHINE, RCRegString, "Version", str))
      VSCANF(str, "%d.%d.%d", &rcMajor, &rcMinor, &rcRevision);
    return true;
  }

  ret = regKeyString(HKEY_LOCAL_MACHINE, MCRegString, "Path", Path);
  if (ret) {
    VSTRCAT(Path, "\\RemoteControl", MAX_PATH);
    if (regKeyString(HKEY_LOCAL_MACHINE, MCRegString, "Version", str))
      VSCANF(str, "%d.%d.%d", &rcMajor, &rcMinor, &rcRevision);
    return true;
  }

  ret = regKeyString(HKEY_LOCAL_MACHINE, RMRegString, "Path", Path);
  if (ret) {
    if (regKeyString(HKEY_LOCAL_MACHINE, RMRegString, "Version", str))
      VSCANF(str, "%d.%d.%d", &rcMajor, &rcMinor, &rcRevision);
    return true;
  }

  return false;
}


BOOL vRemoteCenter::BuildConfig() {
  char path[MAX_PATH] = "";

  rcFound = GetPath(path);
  if (!rcFound) {
    CONSOLE_ACTIVE->printError(getName(), "Remote Center not found");
    return FALSE;
  }

  if (*path) {
    VSTRCAT(path, "\\RCMan.CFG", sizeof(path));
    EditConfig(path);
  }

  long nPathID; 
  ITEMIDLIST* idList; 
  nPathID = SHGetSpecialFolderLocation(0, CSIDL_PERSONAL, &idList); 

  if (SHGetPathFromIDListA(idList, path) == TRUE) {
    VSTRCAT(path, "\\RCMan.CFG", sizeof(path));
    if (VFEXISTS(path)) EditConfig(path);
  }
  
  MakeKeyFile();
  return TRUE;
}


void vRemoteCenter::writeConfig(UINT id, const char *name, const char *file) {
  char section[32];
  VPRINTF(section, sizeof(section), "KeyEvent%d", id);
  char value[16];
  VPRINTF(value, sizeof(value), "%d", id);

  WritePrivateProfileStringA(section, "KeyName",       name,  file);
  WritePrivateProfileStringA(section, "Notification",  "2",   file);
  WritePrivateProfileStringA(section, "HandleByApp",   "1",   file);
  WritePrivateProfileStringA(section, "KeyEventRemap", value, file);
}

/*
[KeyEvent6955]
KeyName=Close/Cancel/Exit
Notification=2
HandleByApp=1
KeyEventRemap=6955
*/

void vRemoteCenter::MakeKeyFile() {
  char thePath[MAX_PATH] = "";
  int  i = 0;

  if (GetPath(thePath)) {
    VSTRCAT(thePath, "\\KeyMap\\", sizeof(thePath));
    VSTRCAT(thePath, FILENAME    , sizeof(thePath));

    WritePrivateProfileStringA("RC Key Map Information", "Name", "RMX Automation", thePath);
    WritePrivateProfileStringA("RC Key Map Information", "Version", "Key " _APP_VERSION, thePath);
    WritePrivateProfileStringA("RC Key Map Information", "By", "Vincent E. Milum Jr. (http://rmx.sourceforge.net)", thePath);
    WritePrivateProfileStringA("RC Key Map Information", "Company", "", thePath);
    WritePrivateProfileStringA("RC Key Map Information", "Copyright", "Copyright (C) 2003-2007 Vincent E. Milum Jr.", thePath);
    WritePrivateProfileStringA("RC Key Map Information", "Comments", "Remote Center support for RMX Automation", thePath);

    cfgBase *child = tree->getFirstChild();
    while (child) {
      writeConfig(child->getParamInt("cmd"), child->getName(), thePath);
      child = tree->getNextChild(child);
    }
  }
}


void vRemoteCenter::onCommand(int id) {
  if (id < 1) return;

  cfgBase *setting = tree->getFirstChild();
  while (setting) {
    if (setting->getParamInt("cmd") == id) {
      evntSingle *s = static_cast<evntSingle*>(setting);
      s->activate();
      break;
    }
    setting = tree->getNextChild(setting);
  }
}


//------------------------------------------------------------------------------------------------


void vRemoteCenter::EditConfig(char *path) {
    //old skewl haxing
  {
    char RCProgs[6][256];
    char str[16];
    int offset = 0;
    int i;

    //old hax
    for (i=0; i<6; i++) {
      VPRINTF(str, sizeof(str), "Priority%X", i+1);
      GetPrivateProfileStringA("PriorityWndClass", str, "", RCProgs[i-offset], 63, path);
      if ( VSTRCMP(RCProgs[i], CLASSNAME) == 0 ) offset++;
    }
    WritePrivateProfileStringA("PriorityWndClass", "Priority1", CLASSNAME, path);

    for (i=0; i<(6-offset); i++) {
      if ( VSTRLEN(RCProgs[i]) > 2 ) {
        VPRINTF(str, sizeof(str), "Priority%X", i+2);
        WritePrivateProfileStringA("PriorityWndClass", str, RCProgs[i], path);
      }
    }

      
    //new haxtreme
    for (i=0; i<6; i++) {
      VPRINTF(str, sizeof(str), "Window%X", i+1);
      GetPrivateProfileStringA("PriorityWndClass", str, "", RCProgs[i-offset], 63, path);
      if ( VSTRCMP(RCProgs[i], CLASSNAME) == 0 ) offset++;
    }
    WritePrivateProfileStringA("PriorityWndClass", "Window1", CLASSNAME, path);

    for (i=0; i<(6-offset); i++)
    {
      if ( VSTRLEN(RCProgs[i]) > 2 )
      {
        VPRINTF(str, sizeof(str), "Window%X", i+2);
        WritePrivateProfileStringA("PriorityWndClass", str, RCProgs[i], path);
      }
    }
  }
  // end old skewl haxing


  //new skewl haxing
  {
    char RCApp[16][256];
    char RCExe[16][256];
    char RCPrm[16][256];
    char RCSrt[16][256];
    char str[16];
    int offset = 0;
    int i;
    int TotalApps = 0;

    TotalApps = GetPrivateProfileIntA("DefApp", "TotalApps", 0, path);

    for (i=0; i<16; i++) {
      VPRINTF(str, sizeof(str), "App%X", i);
      GetPrivateProfileStringA("DefApp", str, "", RCApp[i-offset], 63, path);
      VPRINTF(str, sizeof(str), "Exe%X", i);
      GetPrivateProfileStringA("DefApp", str, "", RCExe[i-offset], 63, path);
      VPRINTF(str, sizeof(str), "Param%X", i);
      GetPrivateProfileStringA("DefApp", str, "", RCPrm[i-offset], 63, path);
      VPRINTF(str, sizeof(str), "StartIn%X", i);
      GetPrivateProfileStringA("DefApp", str, "", RCSrt[i-offset], 63, path);

      if ( VSTRCMP(RCApp[i], CLASSNAME) == 0 ) {
        offset++;
        TotalApps--;
      }
    }

    VPRINTF(str, sizeof(str), "%X", ++TotalApps);
    WritePrivateProfileStringA("DefApp", "TotalApps", str, path);
    WritePrivateProfileStringA("DefApp", "RunDefPlayer", "1", path);
    WritePrivateProfileStringA("DefApp", "CurrentDef", "0", path);

    //TODO: pull the EXE path from RM-X
    WritePrivateProfileStringA("DefApp", "App0", CLASSNAME, path);
    WritePrivateProfileStringA("DefApp", "Exe0", "C:\\Program Files\\RM-X Stand-Alone\\RMX SA.exe", path);

    for (i=0; i<TotalApps; i++) {
      if ( VSTRLEN(RCApp[i]) > 2 ) {
        VPRINTF(str, sizeof(str), "App%X", i+1);
        WritePrivateProfileStringA("DefApp", str, RCApp[i], path);
        VPRINTF(str, sizeof(str), "Exe%X", i+1);
        WritePrivateProfileStringA("DefApp", str, RCExe[i], path);
        VPRINTF(str, sizeof(str), "Param%X", i+1);
        WritePrivateProfileStringA("DefApp", str, RCPrm[i], path);
        VPRINTF(str, sizeof(str), "StartIn%X", i+1);
        WritePrivateProfileStringA("DefApp", str, RCSrt[i], path);
      }
    }

  }
  //end new skewl haxing


  //new skewl haxing2
  {
    char RCApp[16][256];
    char RCExe[16][256];
    char RCPrm[16][256];
    char RCSrt[16][256];
    char str[16];
    int offset = 0;
    int i;
    int TotalApps = 0;

    TotalApps = GetPrivateProfileIntA("WinApp", "TotalApps", 0, path);

    for (i=0; i<16; i++) {
      VPRINTF(str, sizeof(str), "App%X", i);
      GetPrivateProfileStringA("WinApp", str, "", RCApp[i-offset], 63, path);
      VPRINTF(str, sizeof(str), "Exe%X", i);
      GetPrivateProfileStringA("WinApp", str, "", RCExe[i-offset], 63, path);
      VPRINTF(str, sizeof(str), "Param%X", i);
      GetPrivateProfileStringA("WinApp", str, "", RCPrm[i-offset], 63, path);
      VPRINTF(str, sizeof(str), "StartIn%X", i);
      GetPrivateProfileStringA("WinApp", str, "", RCSrt[i-offset], 63, path);

      if ( VSTRCMP(RCApp[i], CLASSNAME) == 0 ) {
        offset++;
        TotalApps--;
      }
    }

    VPRINTF(str, sizeof(str), "%X", ++TotalApps);
    WritePrivateProfileStringA("WinApp", "TotalApps", str, path);
    WritePrivateProfileStringA("WinApp", "RunDefPlayer", "1", path);
    WritePrivateProfileStringA("WinApp", "CurrentDef", "0", path);

    //TODO: pull the EXE path from RM-X
    WritePrivateProfileStringA("WinApp", "App0", CLASSNAME, path);
    WritePrivateProfileStringA("WinApp", "Exe0", "C:\\Program Files\\RM-X Stand-Alone\\RMX SA.exe", path);

    for (i=0; i<TotalApps; i++) {
      if ( VSTRLEN(RCApp[i]) > 2 ) {
        VPRINTF(str, sizeof(str), "App%X", i+1);
        WritePrivateProfileStringA("WinApp", str, RCApp[i], path);
        VPRINTF(str, sizeof(str), "Exe%X", i+1);
        WritePrivateProfileStringA("WinApp", str, RCExe[i], path);
        VPRINTF(str, sizeof(str), "Param%X", i+1);
        WritePrivateProfileStringA("WinApp", str, RCPrm[i], path);
        VPRINTF(str, sizeof(str), "StartIn%X", i+1);
        WritePrivateProfileStringA("WinApp", str, RCSrt[i], path);
      }
    }

  }
  //end new skewl haxing2
}


//-----------------------------------------------------------------------------------------------


vRemoteCenterWnd::vRemoteCenterWnd() : wndBase(CLASSNAME) {
  setName(getClassName());
  setWindowStyle(WS_POPUP);
  setWindowStyleEx(WS_EX_TOOLWINDOW);
}


vRemoteCenterWnd::~vRemoteCenterWnd() {
}


void vRemoteCenterWnd::onInit() {
  wndBase::onInit();

  moveresize(-1, -1, 0, 0);
  showWindow();
}


VUINT vRemoteCenterWnd::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == WM_COMMAND) {
    remoteCenter->onCommand(LOWORD(param1));
    return 0;
  }
  return vMessageWnd::wndProc(msg, param1, param2);
}
