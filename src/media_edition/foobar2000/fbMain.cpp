/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


//todo: enumerate foobar menu items, and expose them to an rmx interface


#include "../../../sdk/rmxBase.h"


#ifdef RMX_FOOBAR2000


#pragma comment(lib, "C:\\Vostro\\Programming\\rmx\\src\\media_edition\\foobar2000\\shared\\shared.lib")


#include "fbMain.h"
#include "fbPlaylistPlugin.h"
#include "../playlistFunction.h"
#include "../common.h"
#include "../remote/remote.h"

//this HAS to come after "main.h"
//#include <../Foobar SDK/Foobar2000/helpers/playback_order_helper.h>



DECLARE_COMPONENT_VERSION(APPNAME, _APP_VERSION, "");

#ifdef _RMX_MEDIA_REMOTE_
VALIDATE_COMPONENT_FILENAME("foo_RMX_Remote.dll");
#else //_RMX_MEDIA_REMOTE_
VALIDATE_COMPONENT_FILENAME("foo_RMX.dll");
#endif //_RMX_MEDIA_REMOTE_


static initquit_factory_t<vFoobarMain> fooMain;
static mainmenu_commands_factory_t<fooMenu> foo_menu;



vFoobarMain::vFoobarMain() {
}


vFoobarMain::~vFoobarMain() {
}


void vFoobarMain::on_init() {
  RMXOS::setInstance( core_api::get_my_instance() );

  UINT tick = RMXOS::getTick();
  rmx = new fooPlugins();
  rmx_init(tick);
}


void vFoobarMain::on_quit() {
  rmx_quit();
}


//-----------------------------------------------------------------------------------------------------


fooPlugins::fooPlugins() {
  setName(APPVERSION);
  udpremote = new rmxUdpRemote();
}


fooPlugins::~fooPlugins() {
  delete udpremote;
  udpremote = NULL;
}


void fooPlugins::onInit() {
  vRmxWin32::onInit();

  char rmxpath[MAX_PATH] = "";
  VPRINTF(rmxpath, sizeof(rmxpath), "%s\\%s", RMXOS::getPath(), RMXOS::getName());

  fbPlaylistPlugin *playlist = new fbPlaylistPlugin(rmxpath);
  appendPlugin(playlist);
}


void fooPlugins::initMasterLists(vLocalPlugin *master) {
  cfgBase *ident = master->getID();
  (ident->insertParam("host", "foobar2000"))->setReadOnly(TRUE);
  ident->insertParamNoSave(   "title",    APPVERSION);
  ident->insertParamNoSave(   "appname",  APPNAME);
  ident->insertParamNoSave(   "version", _APP_VERSION);
  ident->insertParamIntNoSave("build",   _SDK_BUILD);


  vRmxWin32::initMasterLists(master);

  
  //todo:  move this into the main playlist plugin stuff
  funcGroup *playback = new funcGroup("Playback", "player", master);

  //Playback
  new vPlaylistFunction("Play",               "play",       1, playback);
  new vPlaylistFunction("Play/Pause",         "playpause",  2, playback);
  new vPlaylistFunction("Pause",              "pause",      3, playback);
  new vPlaylistFunction("Stop",               "stop",       4, playback);
  new vPlaylistFunction("Stop After Current", "stopafter",  5, playback);
  new vPlaylistFunction("Previous Track",     "prev",      10, playback);
  new vPlaylistFunction("Next Track",         "next",      11, playback);
  new vPlaylistFunction("Random Track",       "randtrack", 12, playback);
  new fooPlaybackOrder(playback);
  new vPlaylistFunction("Fast Forward",       "ffwd",      20, playback);
  new vPlaylistFunction("Fast Rewind",        "frwd",      21, playback);
 

  //volume
  funcGroup *volume = new funcGroup("Volume", "volume", master);
  new vPlaylistFunction("Up",   "up",   30, volume);
  new vPlaylistFunction("Down", "down", 31, volume);
  new vPlaylistFunction("Mute", "mute", 32, volume);
}



//-----------------------------------------------------------------------------------------------------


void fooMenu::get_name(t_uint32 p_index, pfc::string_base & p_out) {
  if (p_index == 0) p_out = rmx->getMasterPlugin()->getID()->getParamSafe("appname");
}


bool fooMenu::get_description(t_uint32 p_index, pfc::string_base & p_out) {
  if (p_index == 0) {
    p_out = rmx->getMasterPlugin()->getID()->getParamSafe("title");
    return true;
  }
  return false;
}


void fooMenu::execute(t_uint32 p_index, service_ptr_t<service_base> p_callback) {
  if (p_index == 0 && core_api::assert_main_thread()) rmx_config();
}


//-----------------------------------------------------------------------------------------------------


fooPlaybackOrder::fooPlaybackOrder(funcGroup *parent) : funcSingle("Playback Order", "order", parent) {
  options = new funcParam(getName(), "f_order", "stringlist", this);
  options->setDefaultValue("Default");
/*  
  service_enum_t<playback_flow_control> e;
  playback_flow_control * ptr;
  for (ptr = e.first(); ptr != 0; ptr = e.next()) {
//    ptr->service_add_ref();
    options->addListItem(ptr->get_name());
    ptr->service_release();  //TODO:  look into if this is needed or not
  }*/
}


void fooPlaybackOrder::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
/*  vPlugin *plugin = plg->getPlugin(source);
  if (!plugin) return;
  const char *f_order = plugin->list_getParam(input, "f_order");
  if (f_order) playback_order_helper::set_config_by_name(f_order);*/
}


//these cause warnings being here, but cause errors without
//SHARED_EXPORT uCallStackTracker::uCallStackTracker(char const *) {}
//SHARED_EXPORT uCallStackTracker::~uCallStackTracker(void) {}
//BOOL SHARED_EXPORT uFormatSystemErrorMessage(pfc::string_base & p_out,DWORD p_code) { return FALSE; }


#endif //RMX_FOOBAR2000
