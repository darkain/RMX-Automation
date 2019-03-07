/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


//todo: poll joyGetNumDevs for device adding/removing

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


#include "Gaming.h"
#include "GameDevice.h" 


vGaming *gaming = new vGaming();


BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE* instance, void* context) {
  return gaming->enumCallback(instance);
}



vGaming::vGaming() : plugBase("Gaming Devices") {
  tree = NULL;
  wnd  = NULL;
}


vGaming::~vGaming() {
  vdelnull(tree);
  vrelease(di);
  vdelnull(wnd);
}


void vGaming::onInit() {
  if (vWin32::isWinNT4()) {
    CONSOLE_ACTIVE->printError(getName(), "Not supported on this OS");
    return;
  }

  CONSOLE_MAIN->print(getName(), "Starting");

  tree = new vGameTree(getName());

  wnd = new vMessageWnd("vMessageWnd");
  wnd->init();

  HRESULT hr = DirectInputCreate(vWin32::getInstance(), DIRECTINPUT_VERSION, (LPDIRECTINPUT*)&di, NULL);
  if (FAILED(hr)) {
    CONSOLE_ACTIVE->printError(getName(), "Failed to initialize DirectInput");
    return;
  }
  
  if (detect()) {
    CONSOLE_ACTIVE->print(getName(), "Started Successfully");
  }
}


void vGaming::onQuit() {
  vdelnull(tree);
  vrelease(di);
  vdelnull(wnd);
}


vGameDevice *vGaming::getDevice(GUID guid) {
  vGameDevice *dev = devices.getFirstItem();
  while (dev) {
    if (rmxGuid::guidcmp(dev->getGuid(), guid) == 0) return dev;
    dev = devices.getNextItem(dev);
  }
  return NULL;
}


BOOL vGaming::detect() {
  CONSOLE_ACTIVE->print(getName(), "Detecting devices");
  HRESULT hr = di->EnumDevices(DIDEVTYPE_JOYSTICK, EnumJoysticksCallback, this, DIEDFL_INCLUDEPHANTOMS | DIEDFL_INCLUDEALIASES);
  if (FAILED(hr)) {
    CONSOLE_ACTIVE->printError(getName(), "Failed to aquire a list of devices");
    return FALSE;
  }
  return TRUE;
}


BOOL vGaming::enumCallback(const DIDEVICEINSTANCE* instance) {
  vGameDevice *dev = getDevice(instance->guidInstance);
  if (dev) {
    dev->setInstance(instance);
    return DIENUM_CONTINUE;
  }

  char str[64];
  char str2[1024];
  VPRINTF(str2, sizeof(str2), "New device: %s", instance->tszProductName);
  CONSOLE_ACTIVE->print(getName(), str2);
  dev = new vGameDevice(instance, tree, rmxGuid::guidToChar(instance->guidInstance, str));
  return DIENUM_CONTINUE;
}


//----------------------------------------------------------------------------------------------------------


void vGameDetect::onClick(HPLUGIN source) {
  gaming->detect();
}


//----------------------------------------------------------------------------------------------------------


vGameTree::vGameTree(const char *name) : cfgTree(name) {
  new vGameDetect(this);
  new cfgBool("Hide detached devices", this);
}


vGameTree::~vGameTree() {
}


cfgBase *vGameTree::onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist) {
  const char *unique = paramlist->getParamSafe("unique");

  if (rmxGuid::isValidGuid( rmxGuid::guidFromChar(unique) )) {
    vGameDevice *dev = new vGameDevice(NULL, this, unique);
    dev->setName(name);
    return dev;
  }
  return cfgTree::onNewExternalChild(name, type, paramlist);
}
