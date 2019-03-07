/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/plugBase.h"
#include "../../sdk/cfg/config.h"
#include "../../sdk/wnd/wndTab.h"
#include "../../sdk/wnd/wndList.h"
#include "../../sdk/wnd/wndFrame.h"  //TODO: actually implement this


INT_PTR CALLBACK dlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);


class vPlaylistWnd;


class vPlaylistPlugin : public plugBase {
  public:
    vPlaylistPlugin();

  protected:
    virtual void onInit();
    virtual void onQuit();

    virtual void onPostInit();


//    void fillList();
//    void addPlaylistItem(const char *name, HWND hlist=NULL);

//    void addTab(const char *name);

  public:
//    inline HWND getWnd() const { return wnd; }

  private:
    cfgTree *root;
    vPlaylistWnd *wnd;
//    HWND wnd;
//    BOOL haslist;
};



class vPlaylistWnd : public wndBase, public cfgBool {
  public:
    vPlaylistWnd(const char *name, cfgBase *parent);
    virtual ~vPlaylistWnd();

  public:
    virtual void onInit();
    virtual void onResize(int width, int height);

	virtual void cb_cfgMessage(cbMessage message, cfgBase *item, const prmBase *param, const prmBase *insert=NULL);
//    virtual void onSetUserData(const prmBase *newdata);


    void refreshList();
//    void updateList();

    inline wndTab  *getTabWnd() const { return tabwnd; }
    inline wndList *getListWnd() const { return listwnd; }

  private:
    wndTab  *tabwnd;
    wndList *listwnd;
};


class vPlaylistTabItem : public wndTabItem {
  public:
    vPlaylistTabItem(const char *name, HPLUGIN hplugin, HLIST hlist, wndTab *parent);
    virtual ~vPlaylistTabItem();

  protected:
    virtual void onSelected();

  public:
    void refreshList(wndList *listwnd);

    inline HPLUGIN getPluginHandle() const { return plugin; }
    inline HLIST getListHandle() const { return list; }

  private:
    HPLUGIN plugin;
    HLIST list;
};


class vPlaylistListItem : public wndListItem {
  public:
    vPlaylistListItem(const char *name, HPLUGIN hplugin, HLIST hlist, wndList *parent);
    virtual ~vPlaylistListItem();

  protected:
    virtual void onDoubleClick();

  public:
    inline HPLUGIN getPluginHandle() const { return plugin; }
    inline HLIST getListHandle() const { return list; }

  private:
    HPLUGIN plugin;
    HLIST list;
};