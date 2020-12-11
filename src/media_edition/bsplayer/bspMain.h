/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _BSPMAIN_H_
#define _BSPMAIN_H_

 
#include "../../rmx_win32/rmx_win32.h"
#include "../../../sdk/func/funcSingle.h"
#include "../../../sdk/func/funcGroup.h"

#include "bsp.h"
#include "bspplg.h"


extern HWND hMessage;


class bsPlugins : public vRmxWin32 {
  public:
    bsPlugins();
    virtual ~bsPlugins();

  protected:
    virtual void onInit();
    virtual void initMasterLists(vLocalPlugin *master);
};


class bsCommand : public funcSingle {
  public:
    bsCommand(const char *function_name, const char *script_name, int cmd, funcGroup *parent=NULL)
      : funcSingle(function_name, script_name, parent) {
      (insertParamIntNoSave("cmd", cmd))->setReadOnly(TRUE);
    }

    bsCommand(const char *function_name, const char *script_name, int cmd, cfgRoot *parent)
      : funcSingle(function_name, script_name, parent) {
      (insertParamIntNoSave("cmd", cmd))->setReadOnly(TRUE);
    }

    virtual ~bsCommand() {}

    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
      SendMessage(hMessage, WM_BSP_CMD, getParamInt("cmd"), 0);
    }
};


#endif //_BSPMAIN_H_
