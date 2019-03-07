/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/Win32/Timer.h"


struct FRAPS_SHARED_DATA {
   DWORD sizeOfStruct;
   DWORD currentFPS;
   DWORD totalFrames;
   DWORD timeOfLastFrame;
   char gameName[32];
};


class vPerformance : public plugBase {
  public:
    vPerformance();

  protected:
    virtual void onInit();

  public:
    const char *getFrameName();
    int getFrameRate();
    int getFrameTime();

    FRAPS_SHARED_DATA *updateFraps();

  private:
    cfgTree       *tree;
    FRAPS_SHARED_DATA *fsd;
    HMODULE            frapsDLL;

    //this is actually a function pointer
    FRAPS_SHARED_DATA *(WINAPI *FrapsSharedData) ();
};




class vFrapsData : public cfgInt, public vTimer {
  public:
    vFrapsData(cfgBase *parent);
    virtual ~vFrapsData() {}

  protected:
    virtual void onTimer(DWORD systemTime);
};
