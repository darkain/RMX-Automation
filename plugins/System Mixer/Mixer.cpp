/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


//TODO:  these can all be turned into evntAbsolute's now that it exists


#pragma comment(lib, "Winmm.lib")

#include "Mixer.h"


static vMixer *sysmixer = new vMixer();


vMixer::vMixer() : plugBase("System Mixer") {
  root = NULL;
}


void vMixer::onInit() {
  root = new cfgTree(getName());
  root->setSavable(FALSE);

  new vEject("Eject CD-ROM", IOCTL_STORAGE_EJECT_MEDIA);
  new vEject("Inject CD-ROM", IOCTL_STORAGE_LOAD_MEDIA);

  int count = mixerGetNumDevs();
  for (int i=0; i<count; i++) {
    new vLineMixer(root, i);
  }
}


void vMixer::onQuit() {
  delete root;
  root = NULL;
}


//------------------------------------------------------------------------------------------------------------------------------


vLineMixer::vLineMixer(cfgBase *parent, int id) : cfgNull("System Mixer", parent), vMessageWnd("System Mixer") {
  setExpanded(TRUE);
  insertParamIntNoSave("mixer", id);
  init();

  mixerOpen((HMIXER*)&mixer, id, (UINT)getWnd(), NULL, MIXER_OBJECTF_HMIXER | MIXER_OBJECTF_MIXER | CALLBACK_WINDOW);
  mixerGetDevCaps(id, &caps, sizeof(MIXERCAPS));
  rmxToUtf8 name(caps.szPname);
  cfgNull::setName(name.getBuffer());

  MIXERLINE line;
  MIXERCONTROL control;
  MIXERLINECONTROLS linecontrol;

  for (UINT i=0; i<caps.cDestinations; i++) {
    line.cbStruct = sizeof(MIXERLINE);
    line.dwDestination = i;
    mixerGetLineInfo(mixer, &line, MIXER_OBJECTF_HMIXER | MIXER_GETLINEINFOF_DESTINATION);

    // get dwControlID
    linecontrol.cbStruct = sizeof(MIXERLINECONTROLS);
    linecontrol.dwLineID = line.dwLineID;
    linecontrol.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
    linecontrol.cControls = 1;
    linecontrol.cbmxctrl = sizeof(MIXERCONTROL);
    linecontrol.pamxctrl = &control;
    if (mixerGetLineControls(mixer, &linecontrol, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) == MMSYSERR_NOERROR) {
      rmxToUtf8 name(line.szName);
      vVolumeSetting *lineinfo = new vVolumeSetting(name.getBuffer(), this, this, &line, &control, &linecontrol);
      lineinfo->setMinMax(control.Bounds.dwMinimum, control.Bounds.dwMaximum);
      if (i == 0) lineinfo->setExpanded(TRUE);
    }
  }
}


vLineMixer::~vLineMixer() {
}


vVolumeSetting *vLineMixer::getSetting(UINT id) {
  vVolumeSetting *item = settings.getFirstItem();
  while (item) {
    if (item->getId() == id) return item;
    item = settings.getNextItem(item);
  }
  return NULL;
}


VUINT vLineMixer::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == MM_MIXM_CONTROL_CHANGE) {
    vVolumeSetting *setting = getSetting(param2);
    if (setting) setting->update();
  }
  return vMessageWnd::wndProc(msg, param1, param2);
}


//------------------------------------------------------------------------------------------------------------------------------


vVolumeSetting::vVolumeSetting(const char *name, cfgBase *parent, vLineMixer *mixer, MIXERLINE *line, MIXERCONTROL *control, MIXERLINECONTROLS *linecontrol)
 : cfgIntSlider(name, parent) {
  linemixer = mixer;
  id = control->dwControlID;

  linemixer->appendSetting(this);

  mxcd.cbStruct = sizeof(MIXERCONTROLDETAILS);
  mxcd.dwControlID = id;
  mxcd.cChannels = 1;
  mxcd.cMultipleItems = 0;
  mxcd.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
  mxcd.paDetails = &mxcdVolume;

  MIXERLINE childline;
  MIXERCONTROL childcontrol;
  MIXERLINECONTROLS childlinecontrol;

  {
    // get dwControlID
    childlinecontrol.cbStruct = sizeof(MIXERLINECONTROLS);
    childlinecontrol.dwLineID = line->dwLineID;
    childlinecontrol.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
    childlinecontrol.cControls = 1;
    childlinecontrol.cbmxctrl = sizeof(MIXERCONTROL);
    childlinecontrol.pamxctrl = &childcontrol;
    if (mixerGetLineControls(mixer->getMixer(), &childlinecontrol, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) == MMSYSERR_NOERROR) {
      rmxToUtf8 name(childcontrol.szName);
      new cfgBool(name.getBuffer(), this);
    }
  }

  //recursive searching starts here:
  int src = line->cConnections;
  for (int n = 0; n < src; n++) {
    childline.cbStruct = sizeof(MIXERLINE);
    childline.dwDestination = line->dwDestination;
    childline.dwSource = n;

    mixerGetLineInfo(mixer->getMixer(), &childline, MIXER_GETLINEINFOF_SOURCE);

    // get dwControlID
    childlinecontrol.cbStruct = sizeof(MIXERLINECONTROLS);
    childlinecontrol.dwLineID = childline.dwLineID;
    childlinecontrol.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
    childlinecontrol.cControls = 1;
    childlinecontrol.cbmxctrl = sizeof(MIXERCONTROL);
    childlinecontrol.pamxctrl = &childcontrol;

    if (mixerGetLineControls(mixer->getMixer(), &childlinecontrol, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) == MMSYSERR_NOERROR) {
      rmxToUtf8 name(childline.szName);
      vVolumeSetting *v = new vVolumeSetting(name.getBuffer(), this, mixer, &childline, &childcontrol, &childlinecontrol);
      v->setMinMax(childcontrol.Bounds.dwMinimum, childcontrol.Bounds.dwMaximum);
    }

  }
  //no more recursive searching
  
  update();
}


vVolumeSetting::~vVolumeSetting() {
  linemixer->removeSetting(this);
}


void vVolumeSetting::update() {
  if (mixerGetControlDetails(linemixer->getMixer(), &mxcd, MIXER_OBJECTF_HMIXER | MIXER_GETCONTROLDETAILSF_VALUE) == MMSYSERR_NOERROR) {
    setValue(mxcdVolume.dwValue);
  }
}


void vVolumeSetting::onSetUserData(const prmBase *newdata) {
  cfgIntSlider::onSetUserData(newdata);
  mxcdVolume.dwValue = getValue();
  if (mixerSetControlDetails(linemixer->getMixer(), &mxcd, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE) != MMSYSERR_NOERROR) {
    CONSOLE_ACTIVE->printWarning(sysmixer->getName(), "Could not set volume");
  }
}


//------------------------------------------------------------------------------------------------------------------------------


vEject::vEject(const char *name, UINT method) : funcSingle(name, NULL) {
  funcParam *prm = new funcParam("Drive Letter", "f_drive", "stringlist", this);

  m = method;
  char paths[1024];
  GetLogicalDriveStringsA(sizeof(paths), paths);
  if (!*paths) return;

  vNullStringList list(paths);
  for (const char *item=list.getFirstItem(); item; item=list.getNextItem(item)) {
    if (GetDriveTypeA(item) == DRIVE_CDROM) prm->addListItem(item);
  }
}


void vEject::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL single) {
  const char *dl = source->list_getParam(input, "f_drive");
  if (!dl  ||  !*dl) return;

  char path[32];
  VPRINTF(path, sizeof(path), "\\\\.\\%s", dl);

  char *pos = path;
  while (*pos) {
    if (*pos == ':') (*(pos+1)) = NULL;
    pos++;
  }

  vThread *thread = new vEjectThread(path, m);
  thread->Begin(THREAD_PRIORITY_NORMAL);
}


//------------------------------------------------------------------------------------------------------------------------------


vEjectThread::vEjectThread(const char *path, UINT method) : vThread(TRUE) {
  vpath.setName(path);
  m = method;
}


DWORD vEjectThread::ThreadProc() {
  ULONG b=0;
  HANDLE h = CreateFileA(vpath.getName(), GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
  if (h) DeviceIoControl(h, m, 0, 0, 0, 0, &b, 0);
  CloseHandle(h);
  return vThread::ThreadProc();
}

