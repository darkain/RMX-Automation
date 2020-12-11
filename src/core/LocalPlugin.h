/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __LOCALPLUGIN_H__
#define __LOCALPLUGIN_H__


#include "../../sdk/rmxBase.h"
#include "../../sdk/rmxList.h"
#include "../../sdk/cfg/config.h"
#include "Plugin.h"


class vLocalPlugin : public vPlugin, public cfgRoot {
  public:
    vLocalPlugin(const char *name, const char *path);
    virtual ~vLocalPlugin();

  public:
    inline const char *getName() const { return cfgRoot::getName(); }

    virtual BOOL removable() const { return FALSE; }
    virtual const char *getPathRootName() const { return "Embedded"; }
    virtual UINT getStatus() { return 1; }


    //get the root list
    virtual HLIST list_getRoot() const { return getListHandle(); }
};



#define getListFromHandle(list) ((vConfigList*)list)
#define getHandleFromList(list) ((HLIST)list)
#define getParamFromHandle(param) ((vConfigParam*)param)
#define getHandleFromParam(param) ((HPARAM)param)



#endif//__LOCALPLUGIN_H__
