//-----------------------------------------------------------------------------
//
//	RadLight 4.0 Source
//
//	Common types and constants
//
//	Author : Igor Janos
//	Last Update : 05-nov-2004
//
//-----------------------------------------------------------------------------

#include "RadLight.h"


//-----------------------------------------------------------------------------
//
//	TWorkerThread Class
//
//-----------------------------------------------------------------------------


TWorkerThread::TWorkerThread() 
{ 
	m_hThread = NULL; 
}
	
TWorkerThread::~TWorkerThread() 
{ 
	CloseThread(); 
}
	
// create thread
BOOL TWorkerThread::CreateThread() 
{
	TAutoLock	Lck(&m_AccessLock);
	if (ThreadExists() == TRUE) return FALSE;

	// try to create the thread
	DWORD	threadid;
	m_hThread = ::CreateThread(NULL, 0, TWorkerThread::GlobalThreadProc, this, 0, &threadid);
	if (m_hThread == 0) return FALSE;
	return TRUE;
}

// kill thread
void TWorkerThread::CloseThread() 
{
	#pragma warning(disable:4312)	// conversion from 'LONG' to 'PVOID'
	HANDLE hThread = (HANDLE) InterlockedExchangePointer( (void **)&m_hThread, NULL);
	if (hThread != 0) {
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}
}

BOOL TWorkerThread::ThreadExists() 
{ 
	return (m_hThread == 0 ? FALSE : TRUE); 
}

DWORD WINAPI TWorkerThread::GlobalThreadProc(void *pParam) 
{
	TWorkerThread *pThread = (TWorkerThread *)pParam;
	return ( (pThread != NULL) ? pThread->ThreadProc() : 0 );
}

HRESULT TWorkerThread::CallWorker(int Cmd) 
{
	TAutoLock	Lck(&m_AccessLock);
	if (ThreadExists() == FALSE) return E_FAIL;
		
	// set new command and wait for reply
	m_iCommand = Cmd;
	m_ReplyEvent.ResetEv();
	m_CommandRequest.SetEv();
	m_ReplyEvent.Wait();
	return m_Reply;
}

void TWorkerThread::Reply(HRESULT Value) 
{ 
	m_Reply = Value; 
	m_CommandRequest.ResetEv(); 
	m_ReplyEvent.SetEv(); 
}

int TWorkerThread::GetCommand() 
{ 
	m_CommandRequest.Wait(); 
	return m_iCommand; 
}

BOOL TWorkerThread::CheckRequest() 
{
	return m_CommandRequest.Check(); 
}

//-----------------------------------------------------------------------------
//
//  TEvent Class
//
//-----------------------------------------------------------------------------
TEvent::TEvent(BOOL fManualReset) 
{
	m_hEvent = CreateEvent(NULL, fManualReset, FALSE, NULL);
}

TEvent::~TEvent()
{
	CloseHandle(m_hEvent);
}

int TEvent::Wait(DWORD dwTimeOut)
{
	return (WaitForSingleObject(m_hEvent, dwTimeOut) == WAIT_OBJECT_0);
}
	
//-----------------------------------------------------------------------------
//
//  TBaseObject Class
//
//  Supports Interface deriving. But does not keep the eye on reference count
//
//-----------------------------------------------------------------------------

ULONG TBaseObject::AddRef()
{
	return 1;
}

ULONG TBaseObject::Release()
{
	return 1;
}

STDMETHODIMP TBaseObject::QueryInterface(REFIID riid, void **ppv)
{
	CheckPointer(ppv, E_POINTER);

	// we only know about IUnknown
	if (riid == IID_IUnknown) {
		GetInterface( (IUnknown *)this, ppv);
		return NOERROR;
	} else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}
}

//-----------------------------------------------------------------------------
//
//  TRLComObject Class
//
//  C++ CUnknown Clone.
//
//-----------------------------------------------------------------------------

TRLComObject::TRLComObject()
	:
	m_iRefCount(0)
{
}

TRLComObject::~TRLComObject()
{
	// dummy
}
	
STDMETHODIMP TRLComObject::QueryInterface(REFIID riid, void **ppv)
{
	CheckPointer(ppv, E_POINTER);

	// we only know about IUnknown
	if (riid == IID_IUnknown) {
		GetInterface( (IUnknown *)this, ppv);
		return NOERROR;
	} else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}
}

ULONG TRLComObject::AddRef()
{
	return InterlockedIncrement(&m_iRefCount);
}

ULONG TRLComObject::Release()
{
	LONG lRef = InterlockedDecrement(&m_iRefCount);
	if (lRef == 0) {
	    // COM rules say we must protect against re-entrancy.
    	// If we are an aggregator and we hold our own interfaces
        // on the aggregatee, the QI for these interfaces will
	    // addref ourselves. So after doing the QI we must release
    	// a ref count on ourselves. Then, before releasing the
        // private interface, we must addref ourselves. When we do
	    // this from the destructor here it will result in the ref
    	// count going to 1 and then back to 0 causing us to
        // re-enter the destructor. Hence we add an extra refcount here
	    // once we know we will delete the object.
    	// for an example aggregator see filgraph\distrib.cpp.
    	m_iRefCount++;
    	delete this;
    	return ULONG(0);
	}
	return m_iRefCount;
}


STDMETHODIMP TSyncMethod::QueryInterface(REFIID riid, void **ppv)
{
	if (riid == IID_IRLSyncMethod) {
		GetInterface( (IRLSyncMethod *)this, ppv);
		return NOERROR;
	}
	return TRLComObject::QueryInterface(riid, ppv);
}

//-----------------------------------------------------------------------------
//
//	Globals
//
//-----------------------------------------------------------------------------

STDAPI GetInterface(LPUNKNOWN pUnk, void **ppv)
{
    CheckPointer(ppv, E_POINTER);
    *ppv = pUnk;
    pUnk->AddRef();
    return NOERROR;
}

HRESULT GetModuleInterface(IRLKernel *Kernel, REFIID riid, void **ppv)
{
	IRLEnumModules	*Enum = NULL;
	IRLModule *Module = NULL;
	*ppv = NULL;
	if (Kernel == NULL) return E_FAIL;

	// query for the enumerator...
	Kernel->QueryInterface(IID_IRLEnumModules, (void **)&Enum);
	if (Enum == NULL) return E_FAIL;

	Enum->Reset();
	while (Enum->Next(&Module) == NOERROR && *ppv == NULL) {
		// try this one....
		if (FAILED(Module->QueryInterface(riid, ppv))) {
			*ppv = NULL;
		}
		Module->Release();
	}
	Enum->Release();

	// and we're done...
	return (*ppv != NULL ? NOERROR : E_NOINTERFACE);
}
