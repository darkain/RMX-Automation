/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _CONFIGWND_TREE_ROOTCALLBACK_H_
#define _CONFIGWND_TREE_ROOTCALLBACK_H_


#include "ConfigWnd.h"


class vTreeRootListCallback : public cbRoot {
  public:
    vTreeRootListCallback(HPLUGIN plugin, vConfigWnd *configwnd) : cbRoot(plugin) {
      config = configwnd;

      config->appendRootCallback(this);
      HLIST list = plugin->root_getFirstChild();
      while (list) {
        config->enumList(plugin, list);
        list = plugin->root_getNextChild(list);
      }
    }

    virtual ~vTreeRootListCallback() {
      config->removeRootCallback(this);
    }


  public:
    virtual void cb_onInsertChild(HLIST child, HLIST insert) {
      config->enumList(getRemotePlugin(), child);
//TODO: FIX THIS SHIT!!
//      cbRoot::cb_onInsertChild(child, insert);
    }

  private:
    vConfigWnd *config;
};


#endif //_CONFIGWND_TREE_ROOTCALLBACK_H_
