/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _MIXER_H_
#define _MIXER_H_


#include "../../sdk/plugBase.h"
#include "../../sdk/Win32/Thread.h"
#include "../../sdk/StringList.h"
#include "../../sdk/Win32/MessageWnd.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/func/funcSingle.h"
#include <Mmsystem.h>


#define IOCTL_STORAGE_EJECT_MEDIA (0x002D4808)
#define IOCTL_STORAGE_LOAD_MEDIA  (0x002D480C)



class vVolumeSetting;


class vMixer : public plugBase {
  public:
    vMixer();

    virtual void onInit();
    virtual void onQuit();

  private:
    cfgTree *root;
};


class vLineMixer : public cfgNull, public vMessageWnd {
  public:
    vLineMixer(cfgBase *parent, int id);
    virtual ~vLineMixer();

     virtual VUINT wndProc(UINT msg, VUINT param1, VUINT param2);

    inline HMIXEROBJ getMixer() const { return mixer; }

    void appendSetting(vVolumeSetting *setting) { settings.appendItem(setting); }
    void removeSetting(vVolumeSetting *setting) { settings.removeItem(setting); }
    vVolumeSetting *getSetting(UINT id);

  private:
    HMIXEROBJ mixer;
    MIXERCAPS caps;
    rmxList<vVolumeSetting*> settings;
};


class vVolumeSetting : public cfgIntSlider {
  public:
    vVolumeSetting(const char *name, cfgBase *parent, vLineMixer *mixer, MIXERLINE *line, MIXERCONTROL *control, MIXERLINECONTROLS *linecontrol);
    virtual ~vVolumeSetting();

    virtual void onSetUserData(const prmBase *newdata);
    inline UINT getId() const { return id; }
    void update();

  private:
    MIXERCONTROLDETAILS_UNSIGNED mxcdVolume;
    MIXERCONTROLDETAILS mxcd;
    vLineMixer *linemixer;
    UINT id;
};


class vEject : public funcSingle {
  public:
    vEject(const char *name, UINT method);
    virtual ~vEject() {}
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    UINT m;
};


class vEjectThread : public rmxThread {
  public:
    vEjectThread(const char *path, UINT method);
    virtual ~vEjectThread() {}
    virtual int threadProc();

  private:
    rmxName vpath;
    UINT m;
};


#endif //_MIXER_H_
