/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _PLUG_BASE_H_
#define _PLUG_BASE_H_


#include "rmxBase.h"
#include "rmxTypes.h"
#include "Win32/Win32.h"

#include "handles/hplugin.h"
#include "handles/hconsole.h"
#include "handles/hfunction.h"

#include "cfg/cfgRoot.h"


class plugBase : public cfgRoot {
  protected:
    plugBase(const char *name);

  public:
    virtual ~plugBase();


  public:
    virtual void onInit()     {} // do all of your init stuff here
    virtual void onPostInit() {} // do anything that requires other plugins to be inited here
    virtual void onQuit()     {} // called before closing RMX

    virtual void onInitConfig() { onInit(); }  //TODO:  this is a quick hack that needs fixed

    //TODO:  LOLz, actually implement this
    //this will unload your plugin.  WARNING!!  this will UNLOAD YOUR PLUGIN!!!
    //meaning it will no longer be active.
    //if you pass FALSE to this param, the only way for it to be re-enabled is
    //for the user to manually enable it within the plugin management GUI
    void unload(BOOL sessiononly=TRUE);


    //use this to get your DLL's specific instance handle
    inline HINSTANCE getInstance() const { return RMXOS::getInstance(); }

    
    //--------------------------------------------
    //HPLUGIN related functions
    //--------------------------------------------
    HPLUGIN getThisPluginHandle() const;


    //--------------------------------------------
    //other functions
    //--------------------------------------------
    BOOL isLoadingXml() const;
    BOOL isSavingXml()  const;


    //these are going to be replaced by the new vConsole API
    void consoleCommand(const char *command);


    //TODO:  implement dependancies
    inline void addDependancy(const char *depend) { dependancies.appendItem(depend); }
    inline const char *getFirstDependancy() const { return dependancies.getFirstItem(); }
    inline const char *getNextDependancy(const char *depend) const { return dependancies.getNextItem(depend); }


  private:
    rmxList<const char*> dependancies;
};


//this greatly reduces binary size on Visual Studio 6
#if defined (_MSC_VER) && (_MSC_VER <= 1200)
#ifndef _DEBUG
// release optimizations
#pragma optimize("gsy",on)
#pragma comment(linker,"/RELEASE")
// set the 512-byte alignment
#pragma comment(linker,"/opt:nowin98")
#endif  // _DEBUG
#endif //MSCVER < 0x1200


//ensure that we link in the proper rmxlib
#ifndef RMX_LIB

#  if (_MSC_VER <= 1200)
#    ifdef _DEBUG
#      pragma comment(lib, "../../lib/rmx_vs6_d.lib")
#    else
#      pragma comment(lib, "../../lib/rmx_vs6.lib")
#    endif
#  endif

#  if (_MSC_VER < 1600  &&  _MSC_VER > 1200)
#    ifdef _M_X64 
#      ifdef _DEBUG
#        pragma comment(lib, "../../lib/rmx_vs8_x64_d.lib")
#      else
#        pragma comment(lib, "../../lib/rmx_vs8_x64.lib")
#      endif
#    else
#      ifdef _DEBUG
#        pragma comment(lib, "../../lib/rmx_vs8_d.lib")
#      else
#        pragma comment(lib, "../../lib/rmx_vs8.lib")
#      endif
#    endif
#  endif

#  if (_MSC_VER >= 1600)
#    ifdef _M_X64
#      ifdef _DEBUG
#        pragma comment(lib, "../../lib/rmx_vs10_x64_d.lib")
#      else
#        pragma comment(lib, "../../lib/rmx_vs10_x64.lib")
#      endif
#    else
#      ifdef _DEBUG
#        pragma comment(lib, "../../lib/rmx_vs10_d.lib")
#      else
#        pragma comment(lib, "../../lib/rmx_vs10.lib")
#      endif
#    endif
#  endif
#endif //RMX_LIB


#endif //_PLUG_BASE_H_
