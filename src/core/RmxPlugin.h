/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __RMXPLUGIN_H__
#define __RMXPLUGIN_H__


#include "../../sdk/rmxBase.h"
#include "Plugin.h"


class RmxPlugin : public vPlugin {
  public:
    RmxPlugin(const char *fname, HMODULE hmod, RMX_ENTRY ventry);
    virtual ~RmxPlugin();

  public:
    virtual void onInitPlugin()     { RmxNul((VMSG::SYSTEM, 10)); }
    virtual void onPostInitPlugin() { RmxNul((VMSG::SYSTEM,  6)); }

    virtual const char *getPathRootName() const { return "dll"; }

    virtual UINT getStatus() { return status; }

    virtual BOOL removable() const { return TRUE;  }
    virtual BOOL external()  const { return FALSE; }

    virtual void onFirstLoad();
//    virtual void onPreSaveSettings();
//    virtual void onPreLoadSettings();
//    virtual void onSaveSettings();
//    virtual void onLoadSettings();


    BOOL testPlugin();


    virtual HLIST list_getRoot() const { RmxPtr(HLIST, (VMSG::SYSTEM, 11)); }


  private:
    inline int x_Load()   const { RmxInt((VMSG::SYSTEM,  1, _RMX_CORE, (vPlugin*)this, hMod)); }
    inline int x_Unload() const { RmxInt((VMSG::SYSTEM,  2)); }
    inline int x_test()   const { RmxInt((VMSG::SYSTEM, 30)); }

  private:
    UINT status;
};


#endif//__RMXPLUGIN_H__
