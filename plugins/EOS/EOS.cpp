/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#pragma comment(lib, "../../plugins/EOS/EDSDK.lib")


#include "EOS.h"


static vEos *EOS = new vEos();


vEos::vEos() : plugBase("Canon EOS Cameras") {
  inited = FALSE;
}


void vEos::onInit() {
  EdsError err = EDS_ERR_OK;

  if ((err = EdsInitializeSDK()) != EDS_ERR_OK) {
    char str[1024];
    VPRINTF(str, sizeof(str), "Error Initializing : %d", err);
    CONSOLE_ACTIVE->printError("EOS", str);
    return;
  }

  if ((err = EdsSetCameraAddedHandler(_EdsCameraAddedHandler, this)) != EDS_ERR_OK) {
    char str[1024];
    VPRINTF(str, sizeof(str), "Error Registering 'Added' Handler : %d", err);
    CONSOLE_ACTIVE->printError("EOS", str);
    return;
  }

  enumCameras();

  inited = TRUE;
}


void vEos::onQuit() {
  removeAllChildren();
  EdsTerminateSDK();
}


vEosCamera *vEos::getCamera(EdsCameraRef ref) {
  cfgBase *child = getFirstChild();
  while (child) {
    if ( ((EdsCameraRef)(child->getParamInt("eos"))) == ref) return static_cast<vEosCamera*>(child);
    child = getNextChild(child);
  }
  return NULL;
}


void vEos::enumCameras() {
  EdsError err = EDS_ERR_OK;

  EdsCameraListRef cameraList = NULL;
  if ((err = EdsGetCameraList(&cameraList)) != EDS_ERR_OK) {
    char str[1024];
    VPRINTF(str, sizeof(str), "Error Getting Camera List : %d", err);
    CONSOLE_ACTIVE->printError("EOS", str);
    return;
  }

  EdsUInt32 count = 0;
  if ((err = EdsGetChildCount(cameraList, &count)) != EDS_ERR_OK) {
    char str[1024];
    VPRINTF(str, sizeof(str), "Error Getting Camera Count : %d", err);
    CONSOLE_ACTIVE->printError("EOS", str);
    return;
  }

  if (count < 1) {
    CONSOLE_ACTIVE->printWarning("EOS", "No Cameras Found");
    return;
  }


  EdsCameraRef camera = NULL;
  for (EdsUInt32 i=0; i<count; i++) {
    if ((err = EdsGetChildAtIndex(cameraList, i ,&camera)) == EDS_ERR_OK) {
      vEosCamera *cam = getCamera(camera);
      if (cam) {
        EdsRelease(camera);
      } else {
        cameras.appendItem(new vEosCamera(this, camera));
      }
    }
  }


  EdsRelease(cameraList);
}



EdsError vEos::_EdsCameraAddedHandler(EdsVoid *inContext) {
  ((vEos*)(inContext))->enumCameras();
  return EDS_ERR_OK;
}


//---------------------------------------------------------------------


vEosCamera::vEosCamera(cfgBase *parent, EdsCameraRef ref) 
 : cfgTree("Digital EOS Camera", parent) {
  camera = ref;

  setSavable(FALSE);
  insertParamInt("eos", (int)(void*)ref);

  function = new vEosFunction(this);


  EdsError err = EDS_ERR_OK;

  if ((err = EdsGetDeviceInfo(camera , &deviceInfo)) == EDS_ERR_OK) {
    CONSOLE_ACTIVE->print("EOS", deviceInfo.szDeviceDescription);
    setName(deviceInfo.szDeviceDescription);
  } else {
    CONSOLE_ACTIVE->printError("EOS", "Error Getting Device Info");
  }

  if ((err = EdsOpenSession(camera)) != EDS_ERR_OK) {
    char str[1024];
    VPRINTF(str, sizeof(str), "Error Opening Camera Session : %d", err);
    CONSOLE_ACTIVE->printError("EOS", str);
    return;
  }

  if ((err = EdsSetPropertyEventHandler(   camera, kEdsPropertyEvent_All, _EdsPropertyEventHandler, this)) != EDS_ERR_OK) {
    char str[1024];
    VPRINTF(str, sizeof(str), "Error Registering Property Event Handler : %d", err);
    CONSOLE_ACTIVE->printError("EOS", str);
  }

  if ((err = EdsSetObjectEventHandler(     camera, kEdsObjectEvent_All,   _EdsObjectEventHandler,   this)) != EDS_ERR_OK) {
    char str[1024];
    VPRINTF(str, sizeof(str), "Error Registering Object Event Handler : %d", err);
    CONSOLE_ACTIVE->printError("EOS", str);
  }

  if ((err = EdsSetCameraStateEventHandler(camera, kEdsStateEvent_All,    _EdsStateEventHandler,    this)) != EDS_ERR_OK) {
    char str[1024];
    VPRINTF(str, sizeof(str), "Error Registering State Event Handler : %d", err);
    CONSOLE_ACTIVE->printError("EOS", str);
  }
}


vEosCamera::~vEosCamera() {
  vdelnull(function);

  EdsError err = EDS_ERR_OK;
  if ((err = EdsCloseSession(camera)) != EDS_ERR_OK) {
    char str[1024];
    VPRINTF(str, sizeof(str), "Error Closing Camera Session : %d", err);
    CONSOLE_ACTIVE->printError("EOS", str);
  }

  EdsRelease(camera);
}



void vEosCamera::onProperyEvent(EdsPropertyEvent inEvent, EdsPropertyID inPropertyID, EdsUInt32 inParam) {
}


void vEosCamera::onObjectEvent(EdsObjectEvent inEvent, EdsBaseRef inRef) {
}


void vEosCamera::onStateEvent(EdsStateEvent inEvent, EdsUInt32 inEventData) {
  if (inEvent == kEdsStateEvent_Shutdown) {
    delete this;
  }
}



EdsError vEosCamera::_EdsPropertyEventHandler(EdsPropertyEvent inEvent, EdsPropertyID inPropertyID, EdsUInt32 inParam, EdsVoid *inContext) {
  char str[1024];
  VPRINTF(str, sizeof(str), "Property Event: %d", inEvent);
  CONSOLE_ACTIVE->print("EOS", str);
  ((vEosCamera*)(inContext))->onProperyEvent(inEvent, inPropertyID, inParam);
  return EDS_ERR_OK;
}


EdsError vEosCamera::_EdsObjectEventHandler(EdsObjectEvent inEvent, EdsBaseRef inRef, EdsVoid *inContext) {
  char str[1024];
  VPRINTF(str, sizeof(str), "Object Event: %d", inEvent);
  CONSOLE_ACTIVE->print("EOS", str);
  ((vEosCamera*)(inContext))->onObjectEvent(inEvent, inRef);
  return EDS_ERR_OK;
}


EdsError vEosCamera::_EdsStateEventHandler(EdsStateEvent inEvent, EdsUInt32 inEventData, EdsVoid *inContext) {
  char str[1024];
  VPRINTF(str, sizeof(str), "State Event: %d", inEvent);
  CONSOLE_ACTIVE->print("EOS", str);
  ((vEosCamera*)(inContext))->onStateEvent(inEvent, inEventData);
  return EDS_ERR_OK;
}



//---------------------------------------------------------------------



void vEosFunction::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  EdsCameraRef ref = camera->getReference();

  EdsError err = EDS_ERR_OK;
  if ((err = EdsSendCommand(ref, kEdsCameraCommand_TakePicture, 0)) != EDS_ERR_OK) {
    char str[1024];
    VPRINTF(str, sizeof(str), "Error Sending Command : %d", err);
    CONSOLE_ACTIVE->printError("EOS", str);
  }
}
