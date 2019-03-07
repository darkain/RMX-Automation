//-----------------------------------------------------------------------------
//
//  RadLight 4.0 Source
//
//  Base Classes Implementation
//
//  Author : Igor Janos
//  Last Update : 30-jul-2003
//
//-----------------------------------------------------------------------------

#ifndef URLBASE_H
#define URLBASE_H


class TRLBaseModule;
class TModuleSyncMethod;
class TLanguageChange;



//-----------------------------------------------------------------------------
//
//	TRLBaseModule Class
//
//-----------------------------------------------------------------------------
class TRLBaseModule : public TRLComObject, 
					  public IRLModule
{
private:
	// necesary interfaces
	IRLKernel		*m_Kernel;
	IRLSettings		*m_Settings;
	IRLLanguage		*m_Language;

	TLock			m_Lock;
	TRLModuleInfo	m_Info;
	int				m_iRunLevel;
	BOOL			m_bProVersion;

	// sync events
	TLanguageChange	*m_pLanguageChange;
	
public:

	TRLBaseModule(IRLKernel *Kernel, TRLModuleInfo Info);
	~TRLBaseModule();

	// events
	virtual void OnLanguageChange() { }
	virtual void OnInitializeInterfaces() { }
	virtual void OnUninitializeInterfaces() { }

	// helper methods
	void SendRLMessage(DWORD dwMessage, WPARAM wParam, LPARAM lParam);
	void Log(LPWSTR ws) { m_Kernel->Log(ws); }

	// --- IUnknown overriden
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef(void) { return TRLComObject::AddRef(); }
	STDMETHODIMP_(ULONG) Release(void) { return TRLComObject::Release(); }

	// --- IRLMessageSink method
	STDMETHODIMP NotifyMessage(TRLMessage *Msg);

	// --- IRLModule methods
	STDMETHODIMP GetModuleInfo(TRLModuleInfo *Info);
	STDMETHODIMP SetRunLevel(int Level);
	STDMETHODIMP Uninitialize();
	STDMETHODIMP Uninit_Status();
	STDMETHODIMP DisplayPropertyPage();
	STDMETHODIMP HidePropertyPage();
	STDMETHODIMP Active();
	STDMETHODIMP Inactive();
	STDMETHODIMP DisplayPropertyPageEx(int Param);

	// handling activation - to avoid fake calls we check for the kernel.
	STDMETHODIMP ActivateProVersion();

	// read-only properties
	IRLKernel *Kernel() { return m_Kernel; }
	TRLModuleInfo *Info() { return &m_Info; }
	TLock *Lock() { return &m_Lock; }
	int RunLevel() { return m_iRunLevel; }
	IRLSettings *Settings() { return m_Settings; }
	IRLLanguage *Language() { return m_Language; }
	BOOL ProVersion() { return m_bProVersion; }
};

//-----------------------------------------------------------------------------
//
//	TModuleSyncMethod Class - wrapper for Modules
//
//-----------------------------------------------------------------------------
class TModuleSyncMethod : public TSyncMethod
{
private:
	TRLBaseModule	*m_pModule;
public:
	TModuleSyncMethod(TRLBaseModule *Module) : m_pModule(Module) { }
	TRLBaseModule *Module() { return m_pModule; }
};


class TLanguageChange : public TModuleSyncMethod
{
public:
	TLanguageChange(TRLBaseModule *Module) : TModuleSyncMethod(Module) { }
	STDMETHODIMP Execute() { Module()->OnLanguageChange(); return NOERROR; }
};


#endif /* URLBASE_H */