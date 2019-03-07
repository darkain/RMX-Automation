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

#include "RadLight.h"


//-----------------------------------------------------------------------------
//
//	TRLBaseModule Class
//
//-----------------------------------------------------------------------------

TRLBaseModule::TRLBaseModule(IRLKernel *Kernel, TRLModuleInfo Info)
	:
	m_Kernel(Kernel),
	m_iRunLevel(LEVEL_NOT_INITIALIZED),
	m_Info(Info)
{
	// let's AddRef our kernel and query for some interfaces
	m_Kernel->AddRef();

	m_Kernel->QueryInterface(IID_IRLLanguage, (void **)&m_Language);
	m_Kernel->QueryInterface(IID_IRLSettings, (void **)&m_Settings);

	// events for Kernel::SynchronizeEx
	m_pLanguageChange = new TLanguageChange(this);
	m_pLanguageChange->AddRef();
}

TRLBaseModule::~TRLBaseModule()
{
	HidePropertyPage();

	// release interfaces we may hold
	INTERFACE_RELEASE(m_Settings);
	INTERFACE_RELEASE(m_Language);
	INTERFACE_RELEASE(m_Kernel);
	INTERFACE_RELEASE(m_pLanguageChange);
}

// helper methods
void TRLBaseModule::SendRLMessage(DWORD dwMessage, WPARAM wParam, LPARAM lParam) 
{
	TRLMessage	Msg;
	ZeroMemory(&Msg, sizeof(Msg));
	Msg.dwMessage = dwMessage;
	Msg.wParam = wParam;
	Msg.lParam = lParam;

	// send it out...
	Kernel()->NotifyMessage(&Msg);
}
	
// --- IUnknown overriden
STDMETHODIMP TRLBaseModule::QueryInterface(REFIID riid, void **ppv) 
{
   	CheckPointer(ppv, E_POINTER);

	// new interfaces - IRLModule, IRLMessageSink
	if (riid == IID_IRLModule) {
		GetInterface( (IRLModule *)this, ppv);
		return NOERROR;
	} else 
	if (riid == IID_IRLMessageSink) {
		GetInterface( (IRLMessageSink *)this, ppv);
		return NOERROR;
	} else 
		return TRLComObject::QueryInterface(riid, ppv);
}

// --- IRLMessageSink method
STDMETHODIMP TRLBaseModule::NotifyMessage(TRLMessage *Msg) 
{
	// handle Language Change Message
	if (Msg->dwMessage == LANGUAGE_CHANGE) {
		Kernel()->SynchronizeEx(m_pLanguageChange);
	}
	return NOERROR;
}


// --- IRLModule methods
STDMETHODIMP TRLBaseModule::GetModuleInfo(TRLModuleInfo *Info) 
{ 
	if (Info) *Info = m_Info; 
	return NOERROR; 
}

STDMETHODIMP TRLBaseModule::SetRunLevel(int Level) 
{
	// NEVER CALL THIS METHOD - ONLY KERNEL CAN DO THIS.
	if (Level == m_iRunLevel) return NOERROR;
	m_iRunLevel = Level;

	if (Level == LEVEL_KERNEL_PROTECTED) {
		HidePropertyPage();
		OnUninitializeInterfaces();
	} else
	if (Level == LEVEL_KERNEL_READY) {
		OnInitializeInterfaces();
	}
	return NOERROR;
}

STDMETHODIMP TRLBaseModule::Uninitialize() 
{ 
	return NOERROR; 
}

STDMETHODIMP TRLBaseModule::Uninit_Status() 
{ 
	return NOERROR; 
}

STDMETHODIMP TRLBaseModule::DisplayPropertyPage() 
{ 
	return DisplayPropertyPageEx(0); 
}

STDMETHODIMP TRLBaseModule::HidePropertyPage() 
{ 
	return E_NOTIMPL; 
}

STDMETHODIMP TRLBaseModule::Active() 
{ 
	return NOERROR; 
}

STDMETHODIMP TRLBaseModule::Inactive() 
{ 
	return NOERROR; 
}

STDMETHODIMP TRLBaseModule::DisplayPropertyPageEx(int Param) 
{ 
	return E_NOTIMPL; 
}

// handling activation - to avoid fake calls we check for the kernel.
STDMETHODIMP TRLBaseModule::ActivateProVersion() 
{
	BOOL Act = FALSE;
	IRLModule *Module = NULL;

	// can only happen in protected mode
	if (RunLevel() != LEVEL_KERNEL_PROTECTED) return E_FAIL;

	Kernel()->GetProVersionActivation(&Act, &Module);
	if (Act == FALSE || Module != this) return E_FAIL;

	// set we're OK
	m_bProVersion = TRUE;
	if (Module) Module->Release();

	return NOERROR;
}

