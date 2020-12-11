/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _RLMAIN_H_
#define _RLMAIN_H_


#include "../../../sdk/func/funcSingle.h"
#include "../../../sdk/func/funcGroup.h"
#include "../../rmx_win32/rmx_win32.h"
#include "RadLight.h"


class rlPlugins;
class rlCategory;
class TPropertyPageMethod;
extern rlPlugins *rlRmx;


class rlPlugins : public TRLBaseModule, public vRmxWin32 {
  public:
    rlPlugins(IRLKernel *Kernel, TRLModuleInfo Info);
    virtual ~rlPlugins();

    virtual void onInit();
    virtual void initMasterLists(vLocalPlugin *master);

    rlCategory *getCategory(UINT rlid);

    //show/hide the config screen
    STDMETHODIMP DisplayPropertyPageEx(int Param);
    STDMETHODIMP HidePropertyPage();

    // InitRoutine
    static HRESULT __stdcall CreateInstance(LPUNKNOWN Kernel, REFIID IID, LPVOID *Obj);
    static HRESULT __stdcall DeleteInstance();

    static IRLKernel *getKernal() { return Kern; }

  private:
    rmxDeleteList<rlCategory*> cats;
    TPropertyPageMethod  *propPage;
    static IRLKernel  *Kern;
};


/*
  "Property Page" aka Configuration
*/
class TPropertyPageMethod : public TModuleSyncMethod {
  public:
    TPropertyPageMethod(TRLBaseModule *Module) : TModuleSyncMethod(Module) { }
    STDMETHODIMP Execute() { Module()->DisplayPropertyPage(); return NOERROR; }
};


class rlCommand : public funcSingle {
  public:
    rlCommand(const char *function_name, const char *script_name, funcGroup *parent, UINT id);
    virtual ~rlCommand() {}

    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    UINT rlid;
};


class rlCategory : public funcGroup {
  public:
    rlCategory(const char *function_name, const char *script_name, UINT id, funcGroup *parent)
      : funcGroup(function_name, script_name, parent) {
      rlid = id;
    }

    virtual ~rlCategory() {}

    UINT getId() { return rlid; }

  private:
    UINT rlid;
};

//-----------------------------------------------------------------------------------------


const  DWORD      MIN_KERNEL_VERSION  = 0x01020000;
const  DWORD      M_COUNT             = 1;
const  WORD       W_DLL_VERSION       = 0x0100;

const TRLModuleInfo RMX_INFO = {
  L"RMX Automation",              // Module Name
  L"RMX Media Edition",           // Description
  L"http://rmx.sourceforge.net/", // URL
  0x01020000,                     // Version
  CAT_ANY,                        // Category
  0xFFFFFFFF,                     // Capability
  rlPlugins::CreateInstance,      // InitRoutine
  NULL, //rlPlugins::DeleteInstance,     // UninitRoutine
  NULL,                           // RegisterActions
  NULL,                           // RegisterExtensions
  TRUE                            // PropertyPage
};


#endif //_RLMAIN_H_
