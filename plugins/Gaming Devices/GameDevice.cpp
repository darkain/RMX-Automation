/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "GameDevice.h"
#include "GameAxis.h"
#include "GameButton.h"
#include "GamePov.h"
#include "GameSlider.h"


//TODO:  change this over to where each instance has all child objects, but hidden
//show child objects upon them being enumerated/found
//this will allow for a consistant visual GUI ordering


vGameDevice::vGameDevice(const DIDEVICEINSTANCE *instance, cfgBase *par, const char *unique)
 : cfgTree("unknown device", par, NULL), vTimer(10, FALSE) {

  insertParam("unique", NULLSTR)->setValue(unique);

  dev  = NULL;
  guid = rmxGuid::guidFromChar(unique);

  guiddisp = new cfgNull(unique, this);
  guiddisp->setSavable(FALSE);

  setInstance(instance);
  gaming->appendDevice(this);
}


vGameDevice::~vGameDevice() {
  gaming->removeDevice(this);
  vrelease(dev);
}


cfgBase *vGameDevice::onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist) {
  cfgBase *ret = NULL;
  const char *unique = paramlist->getParamSafe("unique");

  if (!VSTRCMP(unique, "axis")) {
    vGameAxis *axi = new vGameAxis(_INVALID_GUID, name, this);
    axis.appendItem(axi);
    ret = axi;

  } else if (!VSTRCMP(unique, "pov")) {
    vGamePov *pov = new vGamePov(_INVALID_GUID, name, this);
    povs.appendItem(pov);
    ret = pov;

  } else if (!VSTRCMP(unique, "button")) {
    vGameButton *button = new vGameButton(-1, name, this);
    buttons.appendItem(button);
    ret = button;

  } else if (!VSTRCMP(unique, "slider")) {
    vGameSlider *slider = new vGameSlider(_INVALID_GUID, name, this);
    sliders.appendItem(slider);
    ret = slider;
  }

  if (ret) return ret;
  return cfgTree::onNewExternalChild(name, type, paramlist);
}


void vGameDevice::setInstance(const DIDEVICEINSTANCE *instance) {
  stopTimer();
  if (dev) dev->Release();
  dev = NULL;

  if (!instance) return;
  rmxToUtf8 str(instance->tszProductName);
  setName(str.getBuffer());
//  updateCaps();
  onStart();
}


void vGameDevice::onStart() {
  LPDIRECTINPUT7 di = gaming->getDi();
  HRESULT hr;
  hr = di->CreateDevice(guid, (LPDIRECTINPUTDEVICE*)&dev, NULL);
  hr = dev->SetDataFormat(&c_dfDIJoystick);
  if (FAILED(hr)) return;
  hr = dev->SetCooperativeLevel(gaming->getWnd()->getWnd(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
  if (FAILED(hr)) return;
  dev->Acquire();
  updateCaps();
  
  dev->EnumObjects(vGameDevice::EnumAxesCallback, this, DIDFT_AXIS | DIDFT_POV);

  for (UINT i=0; i<devCaps.dwButtons; i++) {
    if (i >= (UINT)buttons.getItemCount()) {
      char str[32];
      VPRINTF(str, sizeof(str), "Button %d", i+1);
      buttons.appendItem(new vGameButton(i, str, this));
    }
  }

  startTimer();
}


void vGameDevice::onStop() {
}


void vGameDevice::onTimer(DWORD systemTime) {
  if (!dev) {
    stopTimer();
    return;
  }

  dev->Poll();

  DIJOYSTATE devState;
  ZeroMemory(&devState, sizeof(DIJOYSTATE));
  HRESULT hr = dev->GetDeviceState(sizeof(DIJOYSTATE), &devState);
  if (FAILED(hr)) return;

  for (UINT i=0; i<devCaps.dwButtons; i++) {
    vGameButton *btn = getButton(i);
    if (btn) btn->setPressed( devState.rgbButtons[i] );
  }

  vGameAxis *axi;
  axi = getAxis(GUID_XAxis);
  if (axi) axi->setRelativeValue(devState.lX);
  axi = getAxis(GUID_YAxis);
  if (axi) axi->setRelativeValue(devState.lY);
  axi = getAxis(GUID_ZAxis);
  if (axi) axi->setRelativeValue(devState.lZ);
  axi = getAxis(GUID_RxAxis);
  if (axi) axi->setRelativeValue(devState.lRx);
  axi = getAxis(GUID_RyAxis);
  if (axi) axi->setRelativeValue(devState.lRy);
  axi = getAxis(GUID_RzAxis);
  if (axi) axi->setRelativeValue(devState.lRz);

  vGamePov *pov = povs.getFirstItem();
  for (int povcount=0; povcount<4; povcount++) {
    if (pov) pov->setValue(devState.rgdwPOV[povcount]);
    pov = povs.getNextItem(pov);
  }

  vGameSlider *slider = sliders.getFirstItem();
  for (int slidercount=0; slidercount<2; slidercount++) {
    if (slider) slider->setAbsoluteValue(devState.rglSlider[slidercount]);
    slider = sliders.getNextItem(slider);
  }
}


vGameButton *vGameDevice::getButton(int id) {
  vGameButton *btn = buttons.getFirstItem();
  while (btn) {
    if (btn->getId() == id) return btn;
    btn = buttons.getNextItem(btn);
  }
  return NULL;
}


vGameAxis *vGameDevice::getAxis(GUID guid) {
  vGameAxis *axi = axis.getFirstItem();
  while (axi) {
    if (axi->getGuid() == guid) return axi;
    axi = axis.getNextItem(axi);
  }
  return NULL;
}


vGamePov *vGameDevice::getPov(GUID guid) {
  vGamePov *pov = povs.getFirstItem();
  while (pov) {
    if (pov->getGuid() == guid) return pov;
    pov = povs.getNextItem(pov);
  }
  return NULL;
}


vGameSlider *vGameDevice::getSlider(GUID guid) {
  vGameSlider *slider = sliders.getFirstItem();
  while (slider) {
    if (slider->getGuid() == guid) return slider;
    slider = sliders.getNextItem(slider);
  }
  return NULL;
}


void vGameDevice::updateCaps() {
  ZeroMemory(&devCaps, sizeof(devCaps));
  devCaps.dwSize = sizeof(devCaps);
  if (!dev) return;
  dev->GetCapabilities(&devCaps);
}


BOOL vGameDevice::enumAxesCallback(const DIDEVICEOBJECTINSTANCE *object) {
  if (object->guidType != GUID_POV) {
    DIPROPRANGE range;
    ZeroMemory(&range, sizeof(DIPROPRANGE));
    range.diph.dwSize       = sizeof(DIPROPRANGE); 
    range.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
    range.diph.dwHow        = DIPH_BYOFFSET; 
    range.diph.dwObj        = object->dwOfs; // Specify the enumerated axis
    if (object->guidType == GUID_Slider) {
      range.lMin            = 0;
    } else {
      range.lMin            = -10000;
    }
    range.lMax              =  10000;
    dev->SetProperty(DIPROP_RANGE, &range.diph);
  }

  int type = 0;
  
  if      (object->guidType == GUID_XAxis)  type = 1;
  else if (object->guidType == GUID_YAxis)  type = 1;
  else if (object->guidType == GUID_ZAxis)  type = 1;
  else if (object->guidType == GUID_RxAxis) type = 1;
  else if (object->guidType == GUID_RyAxis) type = 1;
  else if (object->guidType == GUID_RzAxis) type = 1;
  else if (object->guidType == GUID_POV)    type = 2;
  else if (object->guidType == GUID_Slider) type = 3;


  if (type == 1) {
    if (!getAxis(object->guidType)) {
      rmxToUtf8 str(object->tszName);
      axis.appendItem(new vGameAxis(object->guidType, str.getBuffer(), this));
    }
  } else if (type == 2) {
    if (!getPov(object->guidType)) {
      rmxToUtf8 str(object->tszName);
      povs.appendItem(new vGamePov(object->guidType, str.getBuffer(), this));
    }
  } else if (type == 3) {
    if (!getSlider(object->guidType)) {
      rmxToUtf8 str(object->tszName);
      sliders.appendItem(new vGameSlider(object->guidType, str.getBuffer(), this));
    }
  }

  return DIENUM_CONTINUE;
}


BOOL CALLBACK vGameDevice::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* objInst, void* pContext) {
  if (!pContext) return DIENUM_CONTINUE;
  vGameDevice *game = (vGameDevice*)pContext;
  return game->enumAxesCallback(objInst);
}
