/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WMPMAIN_H_
#define _WMPMAIN_H_


#include "../../../sdk/func/funcSingle.h"
#include "../../../sdk/func/funcGroup.h"
#include "../resource.h"
#include "../common.h"
#include "wmp.h"
#include "wmpplug.h"


DEFINE_GUID(CLSID_wmpRmx, 0x07B331E1, 0x213E, 0x42E5, 0xB6, 0x3A, 0x9E, 0xE7, 0x7C, 0x49, 0x46, 0x40);

class wmpPlaylistPlugin;


class ATL_NO_VTABLE CwmpRmx : public CComObjectRootEx<CComSingleThreadModel>, public CComCoClass<CwmpRmx, &CLSID_wmpRmx>, public IWMPPluginUI {
  public:
    CwmpRmx();
    ~CwmpRmx();

    DECLARE_REGISTRY_RESOURCEID(IDR_WMPTEST)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(CwmpRmx)
      COM_INTERFACE_ENTRY(IWMPPluginUI)
    END_COM_MAP()

    // CComCoClass methods
    HRESULT FinalConstruct();
    void    FinalRelease();

    // IWMPPluginUI methods
    STDMETHODIMP SetCore(IWMPCore *pCore);
    STDMETHODIMP Create(HWND hwndParent, HWND *phwndWindow);
    STDMETHODIMP Destroy();
    STDMETHODIMP TranslateAccelerator(LPMSG lpmsg);
    STDMETHODIMP DisplayPropertyPage(HWND hwndParent);
    STDMETHODIMP GetProperty(const WCHAR *pwszName, VARIANT *pvarProperty);
    STDMETHODIMP SetProperty(const WCHAR *pwszName, const VARIANT *pvarProperty);

  private:
    void ReleaseCore();
    CComPtr<IWMPCore> m_spCore;
};


class wmpPlugins : public vRmxWin32 {
	public:
		wmpPlugins();
		virtual ~wmpPlugins();

    virtual const char *getHostName();

		virtual void onInit();
    virtual void initMasterLists(vLocalPlugin *master);
};


class wmpFunction : public funcSingle {
  public:
    wmpFunction(const char *function_name, const char *script_name, funcGroup *parent, int cmd);
    virtual ~wmpFunction();

    virtual void onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state);

  private:
    int command;
};


#endif //_WMPMAIN_H_
