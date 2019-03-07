/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


 /*
[17:59:59] <lone> #define IPC_GETNUMAUDIOTRACKS 641
[18:00:03] <lone> #define IPC_GETNUMVIDEOTRACKS 642
[18:00:12] <lone> #define IPC_GETAUDIOTRACK 643
[18:00:15] <lone> #define IPC_GETVIDEOTRACK 644
[18:00:18] <lone> #define IPC_SETAUDIOTRACK 645
[18:00:21] <lone> #define IPC_SETVIDEOTRACK 646
[18:11:49] <lone> VirtualDubMod is a project that was born when suddenly a
  lot of modifications to the original VirtualDub by Avery Lee sprung up,
  mainly on the Doom9 forums. Some people got tired of needing several diffrent
  modified VirtualDub executables, so the idea arose to put them all together
  into a single app, VirtualDubMod.
[18:11:56] <lone> which is what you need to make multiple audio avis ;)
*/


#include "../../../sdk/rmxBase.h"


#ifdef RMX_WINAMP


#include "../../../sdk/Win32/Win32.h"
#include "../playlistFunction.h"
#include "../resource.h"
#include "wa_ipc.h"
#include "waMain.h"
#include "waPlaylistPlugin.h"

#include "../remote/remote.h"



waPlugins *waRmx = NULL;


int waInit();


winampGeneralPurposePlugin plugin = {
  GPPHDR_VER,
  (char*) APPVERSION,
  waInit,
  rmx_config,
  rmx_quit,
};


__declspec( dllexport ) winampGeneralPurposePlugin * winampGetGeneralPurposePlugin() {
  return &plugin;
}


int waInit() {
  UINT tick = RMXOS::getTick();
  RMXOS::setInstance(plugin.hDllInstance);

  //winamp specifics
  waRmx = new waPlugins();
  rmx = waRmx;
  waRmx->setWinamp(plugin.hwndParent);

  rmx_init(tick);

  return 0;
}



//-------------------------------------------------------------------------------------------------


void waCloseWinamp::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  if (waRmx) waRmx->closeWinamp();
}


//-------------------------------------------------------------------------------------------------

waPlugins::waPlugins() {
  setName(_APP_VERSION);
  prevWndProc = NULL;
  wavolume = NULL;
  wabalance = NULL;
  udpremote = NULL;
}

waPlugins::~waPlugins() {
  delete udpremote;
  udpremote = NULL;

  SetWindowLongPtr(getWinamp(), GWLP_WNDPROC, (LONG)prevWndProc);
}


void waPlugins::onInit() {
  vRmxWin32::onInit();

  char rmxpath[MAX_PATH] = "";
  VPRINTF(rmxpath, sizeof(rmxpath), "%s\\%s", RMXOS::getPath(), RMXOS::getName());

  waPlaylistPlugin *playlist = new waPlaylistPlugin(rmxpath);
  appendPlugin(playlist);

  prevWndProc = (WNDPROC)SetWindowLongPtr(getWinamp(), GWL_WNDPROC, (LONG)static_WinampProc);

  udpremote = new rmxUdpRemote();
}


void waPlugins::initMasterLists(vLocalPlugin *master) {
  cfgBase *ident = master->getID();
  (ident->insertParam("host", "Winamp"))->setReadOnly(TRUE);
  ident->insertParamNoSave(   "title",    APPVERSION);
  ident->insertParamNoSave(   "appname",  APPNAME);
  ident->insertParamNoSave(   "version", _APP_VERSION);
  ident->insertParamIntNoSave("build",   _SDK_BUILD);

  vRmxWin32::initMasterLists(master);


  funcGroup *playback = new funcGroup("Playback", "player", master);

  //Playback
  new vPlaylistFunction("Play",               "play",         1, playback, "play");
  new vPlaylistFunction("Play/Pause",         "playpause",    2, playback, "playpause");
  new vPlaylistFunction("Pause",              "pause",        3, playback, "pause");
  new vPlaylistFunction("Stop",               "stop",         4, playback, "stop");
  new vPlaylistFunction("Stop After Current", "stopafter",    5, playback);
  new vPlaylistFunction("Previous Track",     "prev",        10, playback, "prevtrack");
  new vPlaylistFunction("Next Track",         "next",        11, playback, "nexttrack");
  new vPlaylistFunction("Random Track",       "randtrack",   12, playback);
  new vPlaylistFunction("Fast Forward",       "ffwd",        20, playback, "forward");
  new vPlaylistFunction("Fast Rewind",        "frwd",        21, playback, "backward");

  new vPlaylistFunctionHint("Play (hint)",            1, playback, "action0");
  new vPlaylistFunctionHint("Stop (hint)",            4, playback, "action1");
  new vPlaylistFunctionHint("Pause (hint)",           3, playback, "action2");
  new vPlaylistFunctionHint("Previous Track (hint)", 10, playback, "up");
  new vPlaylistFunctionHint("Next Track (hint)",     11, playback, "down");
  new vPlaylistFunctionHint("Fast Forward (hint)",   20, playback, "right");
  new vPlaylistFunctionHint("Fast Rewind (hint)",    21, playback, "left");


  //random/shuffle
  funcGroup *optShuffle = new funcGroup("Shuffle", "shuffle", master);
  new waToggleMessage("Toggle", "toggle", IPC_SET_SHUFFLE, IPC_GET_SHUFFLE, optShuffle);
  new waMessage("Enable",  "enable",  IPC_SET_SHUFFLE, 1, optShuffle);
  new waMessage("Disable", "disable", IPC_SET_SHUFFLE, 0, optShuffle);

  //repeat
  funcGroup *optRepeat = new funcGroup("Repeat", "repeat", master);
  new waToggleMessage("Toggle", "toggle", IPC_SET_REPEAT, IPC_GET_REPEAT, optRepeat);
  new waMessage("Enable",  "enable",  IPC_SET_REPEAT, 1, optRepeat);
  new waMessage("Disable", "disable", IPC_SET_REPEAT, 0, optRepeat);


  //Volume
  funcGroup *volume = new funcGroup("Volume", "volume", master);
  new vPlaylistFunction("Up",   "up",   30, volume, "volup");
  new vPlaylistFunction("Down", "down", 31, volume, "voldown");
  new vPlaylistFunction("Mute", "mute", 32, volume, "volmute");



  funcGroup *song = new funcGroup("Playlist", "playlist", master);
  new waCommand("Open File",    NULL, 40029, song);
  new waCommand("Open URL",     NULL, 40155, song);
  new waCommand("File Info",    NULL, 40188, song);
  new waCommand("Jump To Time", NULL, 40193, song);
  new waCommand("Jump To File", NULL, 40194, song);

  funcGroup *vis = new funcGroup("Visualization", "vis", master);
  new waCommand("Visualiztion Options",          NULL, 40190, vis);
  new waCommand("Visualiztion Plugin Options",   NULL, 40191, vis);
  new waCommand("Configure Visualiztion Plugin", NULL, 40221, vis);
  new waCommand("Start Visualiztion",            NULL, 40192, vis);

  funcGroup *appear = new funcGroup("Appearance", "appearance", master);
  new waCommand("Always On Top",        NULL, 40019, appear);
  new waCommand("Windowshade",          NULL, 40064, appear);
  new waCommand("Playlist Windowshade", NULL, 40266, appear);
  new waCommand("Double-Size",          NULL, 40165, appear);
  new waCommand("Main Window",          NULL, 40258, appear);
  new waCommand("EQ Window",            NULL, 40036, appear);
  new waCommand("Playlist Window",      NULL, 40040, appear);
  new waCommand("Minibrowser Window",   NULL, 40298, appear);
  new waCommand("Skin Selector",        NULL, 40219, appear);
/*
  #define IPC_GETWND_EQ 0 // use one of these for the param
  #define IPC_GETWND_PE 1
  #define IPC_GETWND_MB 2
  #define IPC_GETWND_VIDEO 3
#define IPC_ISWNDVISIBLE 261 // same param as IPC_GETWND
  */

  (new waCommand("Preferences", NULL, 40012, master))->setHint("setup");
  new waCloseWinamp(master);


  //EQ
  cfgTree *winamp = new cfgTree("Winamp", master);


  wavolume  = new waVolume(winamp);
  wabalance = new waBalance(winamp);
//  new cfgIntSlider("Playback", winamp);
  new cfgLine(winamp);

  eq = new waEqBool("Equalizer", winamp, 11);
  autoeq = new waEqBool("Auto", eq, 12);
  waeq.appendItem(new waEqBand("Preamp", eq, 10));
  new cfgLine(eq);
  waeq.appendItem(new waEqBand("60",  eq, 0));
  waeq.appendItem(new waEqBand("170", eq, 1));
  waeq.appendItem(new waEqBand("310", eq, 2));
  waeq.appendItem(new waEqBand("600", eq, 3));
  waeq.appendItem(new waEqBand("1K",  eq, 4));
  waeq.appendItem(new waEqBand("3K",  eq, 5));
  waeq.appendItem(new waEqBand("6K",  eq, 6));
  waeq.appendItem(new waEqBand("12K", eq, 7));
  waeq.appendItem(new waEqBand("14K", eq, 8));
  waeq.appendItem(new waEqBand("16K", eq, 9));

  new waJump("Jump to File", "j", 1, master);
  new waJump("Enqueue File", "e", 2, master);
}


void waPlugins::closeWinamp() {
  winampcommand(40001);
}


UINT waPlugins::winampcommand(UINT command) const {
  return SendMessage(waRmx->getWinamp(), WM_COMMAND, command, 0);
}

UINT waPlugins::winampmessage(UINT command, UINT param) const {
  return SendMessage(waRmx->getWinamp(), WM_WA_IPC, param, command);
}


const char *waPlugins::getHostName() {
  char str[128];
  int ver = SendMessage(getWinamp(), WM_WA_IPC, 0, IPC_GETVERSION);
  int major = HIBYTE(ver);
  int minor = LOBYTE(ver) | (major & 0x0f)<<4;
  VPRINTF(str, sizeof(str), "Nullsoft Winamp - v%x.%02x (%04x)", major>>4, minor, ver);
  return vTimedDelete::create(str);
}



LRESULT CALLBACK waPlugins::static_WinampProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  return waRmx->WinampProc(hwnd, msg, wparam, lparam);
}


LRESULT CALLBACK waPlugins::WinampProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  if (msg == WM_WA_IPC) {
    if (lparam == IPC_PLAYING_FILE) {
      if (plgplaylist) plgplaylist->onSongChange();
    }

    if (lparam == IPC_CB_MISC) {
      if (wparam == IPC_CB_MISC_VOLUME) {
        if (wavolume) wavolume->update();
        if (wabalance) wabalance->update();
      }

      if (wparam == IPC_CB_MISC_EQ) {
        waEqBand *band = waeq.getFirstItem();
        while (band) {
          band->update();
          band = waeq.getNextItem(band);
        }

        if (eq) eq->update();
        if (autoeq) autoeq->update();
      }
    }
  }
  return CallWindowProc(prevWndProc, hwnd, msg, wparam, lparam);
}



//-------------------------------------------------------------------------------------------------


waCommand::waCommand(const char *function_name, const char *script_name, UINT command, funcGroup *parent)
 : funcSingle(function_name, script_name, parent) {
  insertParamIntNoSave("command", command);
}


waCommand::waCommand(const char *function_name, const char *script_name, UINT command, cfgRoot *parent)
 : funcSingle(function_name, script_name, parent) {
  insertParamIntNoSave("command", command);
}


void waCommand::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  if (!rmx) return;
  vPlugin *plugin = rmx->getPlugin(source);
  if (!plugin) return;

  HWND winamp = waRmx->getWinamp();
  SendMessage(winamp, WM_COMMAND, getParamInt("command"), plugin->list_getParamInt(input, "flags"));
}

    
//-------------------------------------------------------------------------------------------------


waMessage::waMessage(const char *function_name, const char *script_name, UINT command, UINT value, funcGroup *parent)
 : funcSingle(function_name, script_name, parent) {
  insertParamIntNoSave("command", command);
  insertParamIntNoSave("value", value);
}


void waMessage::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  if (!rmx) return;
  HWND winamp = waRmx->getWinamp();
  SendMessage(winamp, WM_USER, getParamInt("value"), getParamInt("command"));
}

    
//-------------------------------------------------------------------------------------------------


waToggleMessage::waToggleMessage(const char *function_name, const char *script_name, UINT command, UINT request, funcGroup *parent)
 : funcSingle(function_name, script_name, parent) {
  insertParamIntNoSave("command", command);
  insertParamIntNoSave("request", request);
}


void waToggleMessage::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  if (!rmx) return;
  HWND winamp = waRmx->getWinamp();
  UINT value = !SendMessage(winamp, WM_USER, 0, getParamInt("request"));
  SendMessage(winamp, WM_USER, value, getParamInt("command"));
}

    
//-------------------------------------------------------------------------------------------------


waToggleWindow::waToggleWindow(const char *function_name, const char *script_name, UINT command, UINT wnd_id, UINT toggle, funcGroup *parent)
 : funcSingle(function_name, script_name, parent) {
  insertParamIntNoSave("command", command);
  insertParamIntNoSave("window", wnd_id);
  insertParamIntNoSave("toggle", toggle);
}


void waToggleWindow::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  if (!rmx) return;
  HWND winamp = waRmx->getWinamp();

  if (getParamInt("toggle") == 0) {
    SendMessage(winamp, WM_USER, 0, getParamInt("command"));
  } else {
    UINT value = SendMessage(winamp, WM_USER, getParamInt("window"), IPC_ISWNDVISIBLE);
    if ( ((value==TRUE)&&(getParamInt("Toggle")==1)) || ((value==FALSE)&&(getParamInt("toggle")==2)) ){
      SendMessage(winamp, WM_USER, 0, getParamInt("command"));
    }
  }
}

    
//-------------------------------------------------------------------------------------------------


waEqBand::waEqBand(const char *name, cfgBase *parent, int eqband) : cfgIntSlider(name, parent) {
  band = eqband;
  lock = FALSE;
  setMinMax(0, 63);
  setSavable(FALSE);
  update();
}

void waEqBand::onSetUserData(const prmBase *userdata) {
  cfgIntSlider::onSetUserData(userdata);
  if (!lock) SendMessage(waRmx->getWinamp(), WM_USER, MAKELONG((-getValue())+63, MAKEWORD(band, 0xDB)), 128);
}

void waEqBand::update() {
  int val = -SendMessage(waRmx->getWinamp(), WM_USER, band, 127)+63;
  if (getValue() == val) return;
  lock = TRUE;
  setValue(val);
  lock = FALSE;
}


//-------------------------------------------------------------------------------------------------


waEqBool::waEqBool(const char *name, cfgBase *parent, int eqband) : cfgBool(name, parent) {
  band = eqband;
  lock = FALSE;
  setSavable(FALSE);
  setExpanded(TRUE);
  update();
}

void waEqBool::onSetUserData(const prmBase *userdata) {
  cfgBool::onSetUserData(userdata);
  if (!lock) SendMessage(waRmx->getWinamp(), WM_USER, MAKELONG(getValue(), MAKEWORD(band, 0xDB)), 128);
}

void waEqBool::update() {
  BOOL val = SendMessage(waRmx->getWinamp(), WM_USER, band, 127);
  if (getValue() == val) return;
  lock = TRUE;
  setValue(val);
  lock = FALSE;
}


//-------------------------------------------------------------------------------------------------


waVolume::waVolume(cfgBase *parent) : cfgIntSlider("Volume", parent) {
  lock = FALSE;
  setMinMax(0, 255);
  setSavable(FALSE);
  update();
}

void waVolume::onSetUserData(const prmBase *userdata) {
  cfgIntSlider::onSetUserData(userdata);
  if (!lock) SendMessage(waRmx->getWinamp(), WM_USER, getValue(), 122);
}

void waVolume::update() {
  int val = SendMessage(waRmx->getWinamp(), WM_USER, -(666), 122);
  if (getValue() == val) return;
  lock = TRUE;
  setValue(val);
  lock = FALSE;
}

    
//-------------------------------------------------------------------------------------------------

    
waBalance::waBalance(cfgBase *parent) : cfgIntSlider("Balance", parent) {
  lock = FALSE;
  setMinMax(-128, 127);
  setSavable(FALSE);
  update();
}

void waBalance::onSetUserData(const prmBase *userdata) {
  cfgIntSlider::onSetUserData(userdata);
  if (!lock) SendMessage(waRmx->getWinamp(), WM_USER, getValue(), 123);
}

void waBalance::update() {
  int val = SendMessage(waRmx->getWinamp(), WM_USER, -(666), 123);
  if (getValue() == val) return;
  lock = TRUE;
  setValue(val);
  lock = FALSE;
}


//-------------------------------------------------------------------------------------------------


waJump::waJump(const char *function_name, const char *script_name, UINT id, cfgRoot *parent)
 : funcSingle(function_name, script_name, parent) {
  jid = id;
  new funcParam("Description", "f_desc", "editstring", this);
}


void waJump::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  vPlugin *plugin = rmx->getPlugin(source);
  if (!plugin) return;

  const char *jump = plugin->list_getParam(input, "f_desc");
  if (!jump  ||  !*jump) return;
  rmxUtf8ToWide jumpstr(jump);

  //open jump window
  SendMessage(waRmx->getWinamp(), WM_COMMAND, 40194, 0);
  HWND jumpwnd = FindWindow(NULL, L"Jump to file");

  //TODO: make a check for if the jump window is already in 'jump' or 'enqueue', and adjust as needed
  //"type" in search text
  HWND textwnd = FindWindowEx(jumpwnd, NULL, L"edit", NULL);
  SetWindowText(textwnd, jumpstr.getBuffer());  

  //find the button to press
  HWND button = NULL;
  if (jid == 1) {
    button = FindWindowEx(jumpwnd, NULL, NULL, L"&Jump To File");  //jump
  } else if (jid == 2) {
    button = FindWindowEx(jumpwnd, NULL, NULL, L"En&queue file");  //enqueue
  }

  //press the button to change songs
  SendMessage(button, WM_KEYDOWN, 0x00000020, 0x00390001);
  SendMessage(button, WM_CHAR,    0x00000020, 0x00390001);
  SendMessage(button, WM_KEYUP,   0x00000020, 0xC0390001);
  
  //close the window
  DestroyWindow(jumpwnd);
}


#endif //RMX_WINAMP
