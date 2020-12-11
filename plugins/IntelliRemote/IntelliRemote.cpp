/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#pragma comment(lib,"winmm.lib")


#include "intelliremote.h"



char *stristr (const char *str1, const char *str2) {
  char *cp = (char*) str1;
  char *s1, *s2;

  if (!*str2) return((char*)str1);

  while (*cp) {
    s1 = cp;
    s2 = (char*) str2;
    while ( *s1 && *s2 && !((*s1-*s2) && (*s1-*s2 + 'A' - 'a') && (*s1-*s2 + 'a' - 'A') )) s1++, s2++;
    if (!*s2)
    return(cp);
    cp++;
  }
  return(NULL);
}



static vIntelliRemote *intelliRemote = new vIntelliRemote();


vIntelliRemote::vIntelliRemote() : plugBase("IntelliRemote") {
  tree      = NULL;
  irMidiHdr = NULL;
  irHandle  = NULL;
  irDevNum  = -1;
  irFailed  = FALSE;
  isLoading = FALSE;
  wnd       = NULL;
}



void vIntelliRemote::onInit() {
  tree = new cfgTree(getName());

  devicename = new cfgNull("Device Name: Unassigned", tree);
  devicename->setSavable(FALSE);

  deviceid = new cfgNull("Device ID: Unassigned", tree);
  deviceid->setSavable(FALSE);

  log = new cfgText("Last Command", tree);
  log->setSavable(FALSE);
  log->setEnabled(FALSE);

  new cfgLine(tree);

  new vRemoteEvent("Button 1",          tree, "0A4144512E", "button0");
  new vRemoteEvent("Button 2",          tree, "0A4144710E", "button1");
  new vRemoteEvent("Button 3",          tree, "0941440976", "button2");
  new vRemoteEvent("Button 4",          tree, "094144512E", "button3");
  new vRemoteEvent("Button 5",          tree, "094144215E", "button4");
  new vRemoteEvent("Button 6",          tree, "0941441E61", "button5");
  new vRemoteEvent("Button 7",          tree, "0A4144116E", "button6");
  new vRemoteEvent("Button 8",          tree, "0A4144413E", "button7");
  new vRemoteEvent("Button 9",          tree, "0A41446E11", "button8");
  new vRemoteEvent("Button 0",          tree, "094144017E", "button9");

  new vRemoteEvent("Play/Pause",        tree, "0A41441E61", "playpause");
  new vRemoteEvent("Stop/Eject",        tree, "0A4144215E", "stop");
  new vRemoteEvent("Previous Track",    tree, "0A41447E01", "prevtrack");
  new vRemoteEvent("Next Track",        tree, "0941445E21", "nexttrack");
  new vRemoteEvent("Fast Forward",      tree, "0A41442E51", "forward");
  new vRemoteEvent("Fast Rewind",       tree, "0A4144611E", "backward");
  new vRemoteEvent("Slow",              tree, "0A41443E41");
  new vRemoteEvent("Step",              tree, "0941447E01");
  new vRemoteEvent("Record",            tree, "0A41444E31", "record");
  new vRemoteEvent("Volume Up",         tree, "0941444639", "volup");
  new vRemoteEvent("Volume Down",       tree, "0A41444639", "voldown");
  new vRemoteEvent("Volume Mute",       tree, "0941447609", "volmute");

  new vRemoteEvent("Power",             tree, "0A41440679");
  new vRemoteEvent("Options",           tree, "094144413E", "setup");
  new vRemoteEvent("Display",           tree, "0941446E11", "menu");
  new vRemoteEvent("Return",            tree, "094144710E", "action2");
  new vRemoteEvent("Start",             tree, "094144116E", "action0");
  new vRemoteEvent("Close/Cancel",      tree, "0941443E41" ,"action1");
  new vRemoteEvent("Up Arrow",          tree, "0A41445E21", "up");
  new vRemoteEvent("Down Arrow",        tree, "0A4144314E", "down");
  new vRemoteEvent("Left Arrow",        tree, "0A4144611E", "left");
  new vRemoteEvent("Right Arrow",       tree, "0A41442E51", "right");
  new vRemoteEvent("Left Arrow (XFI)",  tree, "0A41445629" ,"left");
  new vRemoteEvent("Right Arrow (XFI)", tree, "0941444E31", "right");
  new vRemoteEvent("Select/OK",         tree, "0A4144017E", "action0");

  evntBase *parent;  //TODO:  switch these over to "Relative"
  parent = new evntSingle("24Bit Crystalizer", tree);
  parent->insertParamNoSave("key", "0A41440976");
  new vRemoteEvent("Wheel Up",   parent, "0941444936");
  new vRemoteEvent("Wheel Down", parent, "0A41444936");

  parent = new evntSingle("CMSS", tree);
  parent->insertParamNoSave("key", "0A41440E71");
  new vRemoteEvent("Wheel Up",   parent, "0941442956");
  new vRemoteEvent("Wheel Down", parent, "0A41442956");

  parent = new evntSingle("EAX", tree);
  parent->insertParamNoSave("key", "094144314E");
  new vRemoteEvent("Wheel Up",   parent, "0941446916");
  new vRemoteEvent("Wheel Down", parent, "0A41446916");

  parent = new evntSingle("3D MIDI", tree);
  parent->insertParamNoSave("key", "0941441966");
  new vRemoteEvent("Wheel Up",   parent, "0A41441966");
  new vRemoteEvent("Wheel Down", parent, "0941445926");

  
  wnd = new vIntelliRemoteWnd();
  wnd->init();
}


void vIntelliRemote::onPostInit() {
  start();
}


void vIntelliRemote::onQuit() {
  stop();
  vdelnull(wnd);  
}


evntSingle *vIntelliRemote::getEvent(const char *button, cfgBase *parent) {
  if (!parent) parent = tree;
  if (VSTRCMP(button, parent->getParamSafe("key")) == 0) return static_cast<evntSingle*>(parent);

  cfgBase *child = parent->getFirstChild();
  while (child) {
    evntSingle *event = getEvent(button, child);
    if (event) return event;
    child = parent->getNextChild(child);
  }
  return NULL;
}


void vIntelliRemote::setDeviceNumber(int devNum) {
  irDevNum = devNum;
  stop();
  start();
}


void vIntelliRemote::stop() {
  if (!irHandle) return;
  midiInReset(irHandle);
  midiInUnprepareHeader(irHandle, irMidiHdr, sizeof(MIDIHDR));
  midiInClose(irHandle);
  irHandle = NULL;
  free(irMidiHdr->lpData);
  delete irMidiHdr;
  irMidiHdr = NULL;
}


void vIntelliRemote::start() {
  if (isLoading) return;
  isLoading = TRUE;

  char str[1024] = "";

  if (getDeviceNumber() < 0) setDeviceNumber(detectDevice());
  if (getDeviceNumber() >= (int)midiInGetNumDevs()) setDeviceNumber(0);

  VPRINTF(str, sizeof(str), "Device ID: %d", getDeviceNumber());
  deviceid->setName(str);

  
  ZeroMemory(&mic, sizeof(mic));
  midiInGetDevCaps(getDeviceNumber(), &mic, sizeof(mic));

  rmxToUtf8 name(mic.szPname);
  VPRINTF(str, sizeof(str), "Starting on Device %d: %s", getDeviceNumber(), name.getBuffer());
  CONSOLE_ACTIVE->print(getName(), str);

  irFailed = (midiInOpen(&irHandle, getDeviceNumber(), (DWORD_PTR)wnd->getWnd(), 0, CALLBACK_WINDOW)!=MMSYSERR_NOERROR);
  if (irFailed) {
    CONSOLE_ACTIVE->printError(getName(), "Failed on midiInOpen()");
    isLoading = FALSE;
    return;
  }

  irMidiHdr = new MIDIHDR;
  ZeroMemory(irMidiHdr, sizeof(MIDIHDR));
  irMidiHdr->lpData = (char*)malloc(128);
  irMidiHdr->dwBufferLength = 128;

  irFailed = (midiInPrepareHeader(irHandle, irMidiHdr, sizeof(MIDIHDR))!=MMSYSERR_NOERROR);
  if (irFailed) {
    CONSOLE_ACTIVE->printError(getName(), "Failed on midiInPrepareHeader()");
    isLoading = FALSE;
    return;
  }   
  
  irFailed = (midiInAddBuffer(irHandle, irMidiHdr, sizeof(MIDIHDR))!=MMSYSERR_NOERROR);
  if (irFailed) {
    CONSOLE_ACTIVE->printError(getName(), "Failed on midiInAddBuffer()");
    isLoading = FALSE;
    return;
  }   

  midiInStart(irHandle);
  CONSOLE_ACTIVE->print(getName(), "Started Successfully");

  isLoading = FALSE;
}


void vIntelliRemote::handleMidiData(UINT param1, UINT param2) {
  HMIDIIN hMidi = (HMIDIIN)param1;
  
  handleRemoteMem(irMidiHdr->lpData, hMidi);

  midiInUnprepareHeader(hMidi, irMidiHdr, sizeof(MIDIHDR));
  free(irMidiHdr->lpData);
  ZeroMemory(irMidiHdr, sizeof(MIDIHDR));
  irMidiHdr->lpData = (char*)malloc(128);
  irMidiHdr->dwBufferLength = 128;
  midiInPrepareHeader(hMidi, irMidiHdr, sizeof(MIDIHDR));
  midiInAddBuffer(hMidi, irMidiHdr, sizeof(MIDIHDR));
}


int vIntelliRemote::handleRemoteMem(char* c, HMIDIIN hMidi) {
  c += 7;

  char name[32] = "";
  VPRINTF(name, sizeof(name), "%02X%02X%02X%02X%02X", c[0], c[1], c[2], c[3], c[4]);
  log->setText(name);

  evntSingle *event = getEvent(name);
  if (event) {
    event->activate();
    return 0;
  }

  char str[32];
  VPRINTF(str, sizeof(str), "Unknown Button: %s", name);
  CONSOLE_ACTIVE->printWarning(getName(), str);

  return -1;
}


int vIntelliRemote::detectDevice() {
  int ret = 0;
  char str[1024] = "";

  CONSOLE_ACTIVE->print(getName(), "Auto-detecting MIDI Device");
  VPRINTF(str, sizeof(str), "MIDI Devices: %d", midiInGetNumDevs());
  CONSOLE_ACTIVE->print(getName(), str);

  for(UINT i=0; i<midiInGetNumDevs(); i++) {
    midiInGetDevCaps(i, &mic, sizeof(mic));
    rmxToUtf8 name(mic.szPname);
    
    VPRINTF(str, sizeof(str), "MIDI Device %d: %s", i, name.getBuffer());
    CONSOLE_ACTIVE->print(getName(), str);

    if (ret == 0) {
      if (stristr(name.getBuffer(), "live") || stristr(name.getBuffer(), "Extigy")) {
        ret = i;
        VPRINTF(str, sizeof(str), "Device Name: %s", name.getBuffer());
        devicename->setName(str);
      }

      else if (stristr(name.getBuffer(), "Audigy")) {
        ret = i+1;
        midiInGetDevCaps(ret, &mic, sizeof(mic));
        VPRINTF(str, sizeof(str), "Device Name: %s", name.getBuffer());
        devicename->setName(str);
      }
    }
  }
  return ret;
}


//----------------------------------------------------------------------------------------------


vIntelliRemoteWnd::vIntelliRemoteWnd() : vMessageWnd(intelliRemote->getName()) {
}

vIntelliRemoteWnd::~vIntelliRemoteWnd() {
}

VUINT vIntelliRemoteWnd::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == MM_MIM_LONGDATA) {
    intelliRemote->handleMidiData(param1, param2);
  }
  return vMessageWnd::wndProc(msg, param1, param2);
}


//----------------------------------------------------------------------------------------------

/*
char vIntelliRemote::KeyEventValues[33][5] = {
  {0x0A,0x41,0x44,0x51,0x2E}, //00 - 1
  {0x0A,0x41,0x44,0x71,0x0E}, //01 - 2
  {0x09,0x41,0x44,0x09,0x76}, //02 - 3
  {0x09,0x41,0x44,0x51,0x2E}, //03 - 4
  {0x09,0x41,0x44,0x21,0x5E}, //04 - 5
  {0x09,0x41,0x44,0x1E,0x61}, //05 - 6
  {0x0A,0x41,0x44,0x11,0x6E}, //06 - 7
  {0x0A,0x41,0x44,0x41,0x3E}, //07 - 8
  {0x0A,0x41,0x44,0x6E,0x11}, //08 - 9
  {0x09,0x41,0x44,0x01,0x7E}, //09 - 0

  {0x0A,0x41,0x44,0x21,0x5E}, //10 - stop-eject
  {0x0A,0x41,0x44,0x1E,0x61}, //11 - play-pause
  {0x0A,0x41,0x44,0x3E,0x41}, //12 - Slow
  {0x0A,0x41,0x44,0x7E,0x01}, //13 - previous
  {0x09,0x41,0x44,0x5E,0x21}, //14 - next
  {0x09,0x41,0x44,0x7E,0x01}, //15 - step
  {0x09,0x41,0x44,0x76,0x09}, //16 - mute
  {0x0A,0x41,0x44,0x46,0x39}, //17 - volume down
  {0x09,0x41,0x44,0x46,0x39}, //18 - volume up
  {0x09,0x41,0x44,0x31,0x4E}, //19 - eax
  {0x09,0x41,0x44,0x41,0x3E}, //20 - options
  {0x09,0x41,0x44,0x6E,0x11}, //21 - display
  {0x09,0x41,0x44,0x71,0x0E}, //22 - return
  {0x09,0x41,0x44,0x11,0x6E}, //23 - start menu
  {0x09,0x41,0x44,0x3E,0x41}, //24 - close - cancel
  {0x0A,0x41,0x44,0x5E,0x21}, //25 - up
  {0x0A,0x41,0x44,0x61,0x1E}, //26 - left
  {0x0A,0x41,0x44,0x01,0x7E}, //27 - select-ok
  {0x0A,0x41,0x44,0x2E,0x51}, //28 - right
  {0x0A,0x41,0x44,0x31,0x4E}, //29 - down
  {0x0A,0x41,0x44,0x06,0x79}, //30 - power
  {0x0A,0x41,0x44,0x0E,0x71}, //31 - CMSS
  {0x0A,0x41,0x44,0x4E,0x31}, //32 - record
};
*/

/*
All the buttons that are in bold already works.
The 4 wheels on the rm-1800 works the same way as a mouse wheel , we can move it up - down or press it like a button.

+24-BIT CRYSTALIZER Wheel Up 	: {09-41-44-49-36}
+24-BIT CRYSTALIZER Wheel Down 	: {0A-41-44-49-36}
+24-BIT CRYSTALIZER Wheel Press	: {0A-41-44-09-76}

+CMSS-3D Wheel Up			: {09-41-44-29-56}
+CMSS-3D Wheel Down			: {0A-41-44-29-56}
CMSS-3D Wheel Press			: {0A-41-44-0E-71} same function as the old CMSS button

+EAX Wheel Up				: {09-41-44-69-16}
+EAX Wheel Down			: {0A-41-44-69-16}
EAX Wheel Press			: {09-41-44-31-4E} same function as the old EAX button

+3DMIDI Wheel Up			: {0A-41-44-19-66}
+3DMIDI Wheel Down			: {09-41-44-59-26}
+3DMIDI Wheel Press			: {09-41-44-19-66}

---------------------------------------------------------------------------------------

POWER				: {0A-41-44-06-79}

DISPLAY				: {09-41-44-6E-11}

VOLUME -				: {0A-41-44-46-39}
VOLUME +				: {09-41-44-46-39}

MUTE 					: {09-41-44-76-09}

Up Arrow				: {0A-41-44-5E-21}
Down Arrow				: {0A-41-44-31-4E}
*Right Arrow			: {09-41-44-4E-31}
*Left Arrow				: {0A-41-44-56-29}
SELECT/OK				: {0A-41-44-01-7E}

OPTIONS				: {09-41-44-41-3E}

RETURN				: {09-41-44-71-0E}
START					: {09-41-44-11-6E}

CANCEL				: {09-41-44-3E-41}
---------------------------------------------------------------------------------------
+REW					: {0A-41-44-61-1E}
+FWD					: {0A-41-44-2E-51}
PREVIOUS				: {0A-41-44-7E-01}
NEXT					: {09-41-44-5E-21}
REC					: {0A-41-44-4E-31}
Stop/Eject				: {0A-41-44-21-5E}
Play/Pause				: {0A-41-44-1E-61}
SLOW					: {0A-41-44-3E-41}

---------------------------------------------------------------------------------------

1					: {0A-41-44-51-2E}
2					: {0A-41-44-71-0E}
3					: {09-41-44-09-76}
4					: {09-41-44-51-2E}
5					: {09-41-44-21-5E}
6					: {09-41-44-1E-61}
7					: {0A-41-44-11-6E}
8					: {0A-41-44-41-3E}
9					: {0A-41-44-6E-11}
0					: {09-41-44-01-7E}
*/
