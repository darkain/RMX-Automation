/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../../sdk/rmxBase.h"


#ifdef _RMX_XML_SETTINGS_


#include "xmlSettings.h"


xmlSetting::xmlSetting(vPlugin *plug)  {
  plugin = plug;
  section = 0;
  list = 0;

  VPRINTF(path, sizeof(path), "%s\\rmx_settings\\%s.xml", RMXOS::getPath(), plugin->getName());
  if (VFEXISTS(path)) {
    DEBUGMSG("XML", path);
    loadFile(path);
  } else {
    DEBUGMSG("XML", plug->getName());
    if (!plg->isLoaded()) plug->onFirstLoad();
  }
}


xmlSetting::~xmlSetting() {
  xmlItem *item = items.getFirstItem();
  while (item) {
    delete item;
    items.removeItem(item);
    item = items.getFirstItem();
  }
}


void xmlSetting::xml_onStartElement(const char *xmltag, xmlParamList *params) {
  if (getDepth() < 2) return;

  HLIST list = 0;
  const char *name    = params->getItemValue("name");
  const char *expand  = params->getItemValue("expanded");
  if (!expand) expand = "0";

  if (items.hasItems()) {
    list = plugin->list_getChild(  items.getLastItem()->list, name, xmltag, params);
    if (!list) {
      list = plugin->list_newChild(items.getLastItem()->list, name, xmltag, params);
    }
  } else {
    list = plugin->list_getRoot();
  }


  xmlItem *item = new xmlItem();
  item->name.setName(name);
  item->list = list;
  items.appendItem(item);

  if (!list) {
#ifdef use_DebugMessage
    char str[512];
    VPRINTF(str, sizeof(str), "Unrecognized XML Path - Name: %s\nFile: %s", name, path);
    CONSOLE_MAIN->printDebug("XML", str);
//    MessageBox(0, str, "Unrecognized XML Path", 0);
#endif
    return;
  }

  if (!plugin->list_isSavable(list)) return;

  //set expanded property, tho, this is sorta a hack, gosta fix0r
  plugin->list_setExpanded(list, (*expand == '1'));


  //TODO:  possibly pass this job along to the plugin?
  rmxPair<const char*> *param = params->getFirstItem();
  while (param) {
    const char *paramname = param->one;
    const char *paramval  = param->two;

    //things to ignore
    if      (VSTRCMP(paramname, "name")     == 0) {}
    else if (VSTRCMP(paramname, "expanded") == 0) {}

    //things to process
    else {
      if (VSTRCMP(paramname, "id") == 0) {
        int id=0;
        VSCANF(paramval, "%d", &id);
        plugin->addId(id);
      }

      HPARAM paramdata = plugin->list_getParamHandle(list, paramname);
      if (paramdata) {
        if (plugin->param_isSavable(paramdata)) plugin->list_setParam(list, paramname, paramval);
      } else {
        plugin->list_newParam(list, paramname, paramval);
      }
    }

    param = params->getNextItem(param);
  }
}


void xmlSetting::xml_onEndElement(const char *xmltag) {
  if (getDepth() < 2) return;
  xmlItem *list = items.getLastItem();
  items.removeLastItem();
  delete list;
}


void xmlSetting::xml_onCharacterData(const char *str) {
  xmlItem *item = items.getLastItem();
  if (!item  ||  !item->list) return;
  if (!plugin->list_isSavable(item->list)) return;
  plugin->list_setUserData(item->list, str);
}



#endif //_RMX_XML_SETTINGS_
