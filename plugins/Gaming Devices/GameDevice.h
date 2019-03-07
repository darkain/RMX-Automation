/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _GAME_DEVICE_H_
#define _GAME_DEVICE_H_


#include "Gaming.h"


class vGameAxis;
class vGameButton;
class vGamePov;
class vGameSlider;


class vGameDevice : public cfgTree, public vTimer {
  public:
    vGameDevice(const DIDEVICEINSTANCE *instance, cfgBase *par, const char *unique);
    virtual ~vGameDevice();

  public:
    virtual void onStart();
    virtual void onStop();


    vGameButton *getButton(int id);
    vGameAxis   *getAxis(  GUID guid);
    vGamePov    *getPov(   GUID guid);
    vGameSlider *getSlider(GUID guid);

    void setInstance(const DIDEVICEINSTANCE *instance);

    inline GUID getGuid() { return guid; }

  protected:
    virtual void onTimer(DWORD systemTime);

    void updateCaps();

    virtual cfgBase *onNewExternalChild(const char *name, const char *type, HPARAMLIST paramlist);

    static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE* objInst, void* pContext);
    virtual BOOL enumAxesCallback(const DIDEVICEOBJECTINSTANCE *object);

  private:
    GUID guid;
    LPDIRECTINPUTDEVICE7 dev;
    DIDEVCAPS  devCaps;

    rmxList<vGameButton*> buttons;
    rmxList<vGameAxis*>   axis;
    rmxList<vGamePov*>    povs;
    rmxList<vGameSlider*> sliders;
    cfgBase *guiddisp;
};


#endif //_GAME_DEVICE_H_
