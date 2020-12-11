/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _FBMAIN_H_
#define _FBMAIN_H_


#ifndef UNICODE
#define UNICODE
#endif


#include "SDK/Foobar2000.h"
#include "../../rmx_win32/rmx_win32.h"
#include "../../../sdk/func/funcSingle.h"
#include "../../../sdk/func/funcGroup.h"


class rmxUdpRemote;


class vFoobarMain : public initquit {
  public:
    vFoobarMain();
    virtual ~vFoobarMain();

//    virtual int service_release() throw() { return 0; }
//    virtual int service_add_ref() throw() { return 1; }

public:
    virtual void on_init();
    virtual void on_quit();
};

/*
class fooMenu : public menu_item_main_single {
  public:
    virtual void get_name(string_base & out) { out="Components/RM-X Configuration"; }
    virtual void run();
};
*/


class fooMenu : public mainmenu_commands {

  virtual t_uint32 get_command_count() {
    return 1;
  }

  virtual GUID get_command(t_uint32 p_index) {
    static const GUID rmx_menu = { 0x908174ba, 0xf4a5, 0x4dc2, { 0x45, 0x8e, 0xfe, 0x52, 0x3f, 0x18, 0xda, 0x28 } };
    if (p_index == 0) return rmx_menu;
    return pfc::guid_null;
  }

  virtual GUID get_parent() {
    return mainmenu_groups::view;
  }

  virtual void get_name(t_uint32 p_index, pfc::string_base & p_out);
  virtual bool get_description(t_uint32 p_index, pfc::string_base & p_out);
  virtual void execute(t_uint32 p_index, service_ptr_t<service_base> p_callback);
};


class fooPlugins : public vRmxWin32 {
  public:
    fooPlugins();
    virtual ~fooPlugins();

  protected:
    virtual void onInit();
    virtual void initMasterLists(vLocalPlugin *master);

  private:
	rmxUdpRemote *udpremote;
};


//-----------------------------------------------------------------------------------------------------


class fooPlaybackOrder : public funcSingle {
  public:
    fooPlaybackOrder(funcGroup *parent);
    virtual ~fooPlaybackOrder() {}
    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    funcParam *options;
};


#endif //_FBMAIN_H_
