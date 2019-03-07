/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/
#if 0


#include "../../../sdk/rmxBase.h"
#include "ConfigPaneWnd.h"
#include "ConfigPaneItems.h"
#include "ConfigWnd.h"


vConfigPaneWnd::vConfigPaneWnd(vConfigWnd *parent, HPLUGIN cb_plugin, HLIST cb_list)
 : wndForest(parent), cbCore(cb_plugin, cb_list) {
  config = parent;
}


vConfigPaneWnd::~vConfigPaneWnd() {
  message(GENERIC_CLOSE);
  config->setTitle("");
}


void vConfigPaneWnd::onInit() {
  wndForest::onInit();


  HPLUGIN plugin = getRemotePlugin();
  HLIST   list   = getRemoteConfig();
  if (!plugin) return;

  message(GENERIC_OPEN, (RMXLONG)getWnd());

  setTitle(getParam("title"));

  HLIST child = plugin->list_getFirstChild(list);
  while (child) {
    enumChildren(child, this);
    child = plugin->list_getNextChild(list, child);
  }
}


void vConfigPaneWnd::setTitle(const char *text) {
  if (!text || !*text) text = cbCore::getName();
  config->setTitle(text);
}


void vConfigPaneWnd::cb_cfgMessage(cbMessage message, HLIST child, HLIST insert) {
	cbCore::cb_cfgMessage(message, child, insert);

	if (message == CB_CHILD_MOVE) {
		moveForestChild(getObjectFromList(child), getObjectFromList(insert));
	}

	if (message == CB_CHILD_INSERT) {
		enumChildren(child, this, getObjectFromList(insert));
		resizeObjects();
	}
}


void vConfigPaneWnd::cb_cfgMessage(cbMessage message, HPARAM param, HPARAM insert) {
	cbCore::cb_cfgMessage(message, param, insert);

	if (message == CB_PARAM_UPDATED) {
		const char *name = getRemotePlugin()->param_getName(param);
		if (name  &&  *name  &&  VSTRCMP(name, "title") == 0) {
			setTitle(getRemotePlugin()->param_getValue(param));
		}
	}
}


/*
void vConfigPaneWnd::cb_onSetParam(HPARAM param) {
  cbCore::cb_onSetParam(param);
  if (VSTRCMP(getRemotePlugin()->param_getName(param), "title") == 0) {
    setTitle(getRemotePlugin()->param_getValue(param));
  }
}


void vConfigPaneWnd::cb_onInsertChild(HLIST child, HLIST insert) {
  cbCore::cb_onInsertChild(child, insert);
  enumChildren(child, this, getObjectFromList(insert));
  resizeObjects();
}


void vConfigPaneWnd::cb_onMoveChild(HLIST child, HLIST insert) {
  cbCore::cb_onMoveChild(child, insert);
  moveForestChild(getObjectFromList(child), getObjectFromList(insert));
}
*/

void vConfigPaneWnd::enumChildren(HLIST list, wndForestBase *object, wndForestBase *insert) {
  wndForestItem *item = NULL;
  HPLUGIN plugin = getRemotePlugin();
  if (!plugin) return;

  const char *type = plugin->list_getType(list);
  if (VSTRCMP(type, "button") == 0) {
    item = new vConfigPaneButton(this, plugin, list, object, insert);
  } else if (VSTRCMP(type, "event") == 0) {  //TODO: change this to a better "event" type handler
    item = new vConfigPaneButton(this, plugin, list, object, insert);
  } else if (VSTRCMP(type, "string") == 0) {
    item = new vConfigPaneEdit(this, plugin, list, object, insert);
  } else if (VSTRCMP(type, "bool") == 0) {
    item = new vConfigPaneBool(this, plugin, list, object, insert);
  } else if (VSTRCMP(type, "hr") == 0) {
    item = new vConfigPaneLine(this, plugin, list, object, insert);
  } else if (VSTRCMP(type, "path") == 0) {
    item = new vConfigPagePathPicker(this, plugin, list, object, insert);
  } else if (VSTRCMP(type, "link") == 0) {
    item = new vConfigPaneLink(this, plugin, list, object, insert);
  } else if (VSTRCMP(type, "null") == 0) {
    item = new vConfigPaneNull(this, plugin, list, object, insert);

  } else if (VSTRCMP(type, "int") == 0) {
    const char *style = plugin->list_getParamSafe(list, "style");
    if (VSTRCMP(style, "slider") == 0) {
      item = new vConfigPaneSlider(this, plugin, list, object, insert);
    } else if (VSTRCMP(style, "edit") == 0) {
      item = new vConfigPaneEditInt(this, plugin, list, object, insert);
    } else {
      item = new vConfigPaneNull(this, plugin, list, object, insert);
    }
  }

  if (item) {
    item->init();
    if (plugin->list_isVisible(list)) item->showWindow();
    HLIST child = plugin->list_getFirstChild(list);
    while (child) {
      enumChildren(child, item);
      child = plugin->list_getNextChild(list, child);
    }
  }
}


wndForestItem *vConfigPaneWnd::getObjectFromList(HLIST list) const {
  vConfigPaneItem *item = items.getFirstItem();
  while (item) {
    if (item->getRemoteConfig() == list) return item->getForstItem();
    item = items.getNextItem(item);
  }
  return NULL;
}
#endif
