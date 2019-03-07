/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/cfgTree.h"
#include "../../sdk/func/funcSingle.h"
#include "EDSDK.h"


class vEosCamera;


class vEos : public plugBase {
  public:
    vEos();

  protected:
    virtual void onInit();
    virtual void onQuit();

  public:
    vEosCamera *getCamera(EdsCameraRef ref);

  private:
    void enumCameras();

  private:
    static EdsError EDSCALLBACK _EdsCameraAddedHandler(EdsVoid *inContext);

  private:
    BOOL inited;
    rmxList<vEosCamera*> cameras;
};


class vEosCamera : public cfgTree {
  public:
    vEosCamera(cfgBase *parent, EdsCameraRef ref);
    virtual ~vEosCamera();

  protected:
    virtual void onProperyEvent(EdsPropertyEvent inEvent, EdsPropertyID inPropertyID, EdsUInt32 inParam);
    virtual void onObjectEvent(EdsObjectEvent inEvent, EdsBaseRef inRef);
    virtual void onStateEvent(EdsStateEvent inEvent, EdsUInt32 inEventData);

  public:
    inline EdsCameraRef getReference() const { return camera; }


  private:
    static EdsError EDSCALLBACK _EdsPropertyEventHandler(EdsPropertyEvent inEvent, EdsPropertyID inPropertyID, EdsUInt32 inParam, EdsVoid *inContext);
    static EdsError EDSCALLBACK _EdsObjectEventHandler(EdsObjectEvent inEvent, EdsBaseRef inRef, EdsVoid *inContext);
    static EdsError EDSCALLBACK _EdsStateEventHandler(EdsStateEvent inEvent, EdsUInt32 inEventData, EdsVoid *inContext);

  private:
    EdsCameraRef camera;
    EdsDeviceInfo deviceInfo;
    funcSingle *function;
};



class vEosFunction : public funcSingle {
  public:
    vEosFunction(vEosCamera *cam) : funcSingle("Take Picture", NULL) { camera = cam; }
    virtual ~vEosFunction() {}

  protected:
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    vEosCamera *camera;
};
