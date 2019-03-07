/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../core/Loader.h"
#include "../core/LocalPlugin.h"
#include "playlistFunction.h"
#include "pluginPlaylist.h"


//--------------------------------------------------------------------------------------


vPlaylistFunction::vPlaylistFunction(const char *function_name, const char *script_name, int id, funcGroup *parent, const char *hint)
 : funcSingle(function_name, script_name, parent) {
  cmd = id;
  param = NULL;

  if (hint) setHint(hint);

  if ( (id >= 10) && (id <=11) ) {
    param = new funcParam("Tracks to skip", "f_flags", "editintlist", this);
    param->setSuffix(" Track(s)");
    param->setDefaultValueInt(1);
    param->addListItem("1");
    param->addListItem("5");
    param->addListItem("10");
    param->addListItem("25");
    param->addListItem("100");
  }

  else if ( (id >= 20) && (id <=21) ) {
    param = new funcParam("Seek in milliseconds", "f_flags", "editintlist", this);
    param->setSuffix("ms");
    param->setDefaultValueInt(5000);
    param->addListItem("500");
    param->addListItem("1000");
    param->addListItem("5000");
    param->addListItem("10000");
    param->addListItem("30000");
    param->addListItem("60000");
  }

  else if ( (id >= 30) && (id <=31) ) {
    param = new funcParam("Audio Volume in Percentage", "f_flags", "editintlist", this);
    param->setSuffix("%");
    param->setDefaultValueInt(10);
    param->addListItem("1");
    param->addListItem("5");
    param->addListItem("10");
    param->addListItem("25");
  }
}


vPlaylistFunction::~vPlaylistFunction() {
}


void vPlaylistFunction::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  vPlugin *plugin = plg->getPlugin(source);

  int    flagint = (plugin) ? (plugin->list_getParamInt(   input, "f_flags")) : (0);
  double flagnum = (plugin) ? (plugin->list_getParamNumber(input, "f_flags")) : (0);
  if (flagint == 0) flagint = (param) ? (param->getDefaultValueInt()   ) : (0);
  if (flagnum == 0) flagnum = (param) ? (param->getDefaultValueNumber()) : (0);

  if (plgplaylist) switch (cmd) {
    case 1:  plgplaylist->play();               break;
    case 2:  plgplaylist->playPause();          break;
    case 3:  plgplaylist->pause();              break;
    case 4:  plgplaylist->stop();               break;
    case 5:  plgplaylist->stopAfterCurrent();   break;

    case 10: plgplaylist->prev(flagint);        break;
    case 11: plgplaylist->next(flagint);        break;
    case 12: plgplaylist->randTrack();          break;
    
    case 20: plgplaylist->fastForward(flagint); break;
    case 21: plgplaylist->fastRewind( flagint); break;

    case 30: plgplaylist->volumeUp(  flagnum);  break;
    case 31: plgplaylist->volumeDown(flagnum);  break;
    case 32: plgplaylist->volumeMute();         break;
  }
}
