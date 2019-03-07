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

#ifndef URLTYPES_H
#define URLTYPES_H


//-----------------------------------------------------------------------------
//
//	GUIDS
//
//-----------------------------------------------------------------------------

// {05E8EC72-B859-4bc8-9BB5-5955608B33BB}
static const GUID IID_IRLKernel = 
{ 0x05E8EC72, 0xB859, 0x4bc8, { 0x9B, 0xB5, 0x59, 0x55, 0x60, 0x8B, 0x33, 0xBB } };

// {C529D5BE-B263-482a-850A-11EB03FD3322}
static const GUID IID_IRLModule = 
{ 0xC529D5BE, 0xB263, 0x482a, { 0x85, 0x0A, 0x11, 0xEB, 0x03, 0xFD, 0x33, 0x22 } };

// {3309CC21-E148-43b3-8996-4C24E454C642}
static const GUID IID_IRLSettings = 
{ 0x3309CC21, 0xE148, 0x43b3, { 0x89, 0x96, 0x4C, 0x24, 0xE4, 0x54, 0xC6, 0x42 } };

// {8700AC4E-CFE1-4A1D-A281-EB9F3A55EB23}
static const GUID IID_IRLLanguage = 
{ 0x8700AC4E, 0xCFE1, 0x4A1D, { 0xA2, 0x81, 0xEB, 0x9F, 0x3A, 0x55, 0xEB, 0x23 } };

// {FA49E2F5-9D50-41ba-96E0-C607CF63EE58}
static const GUID IID_IRLMessageSink = 
{ 0xFA49E2F5, 0x9D50, 0x41ba, { 0x96, 0xE0, 0xC6, 0x07, 0xCF, 0x63, 0xEE, 0x58 } };

// {8A4D3DC3-1033-4337-B8A4-B78C21B541D2}
static const GUID IID_IRLSyncMethod = 
{ 0x8a4d3dc3, 0x1033, 0x4337, { 0xb8, 0xa4, 0xb7, 0x8c, 0x21, 0xb5, 0x41, 0xd2 } };

// {07EC02DB-2AC5-4171-8438-9F9CD5ABB993}
static const GUID IID_IRLPropertyPage = 
{ 0x07EC02DB, 0x2AC5, 0x4171, { 0x84, 0x38, 0x9F, 0x9C, 0xD5, 0xAB, 0xB9, 0x93 } };

// {8573BA1E-7C43-4980-AF8B-68F9C59F9884}
static const GUID IID_IRLActionList = 
{ 0x8573BA1E, 0x7C43, 0x4980, { 0xAF, 0x8B, 0x68, 0xF9, 0xC5, 0x9F, 0x98, 0x84 } };

// {BE4A8594-1525-4DA4-AA69-69E4C47F0D10}
static const GUID IID_IRLEnumModules = 
{ 0xBE4A8594, 0x1525, 0x4DA4, { 0xAA, 0x69, 0x69, 0xE4, 0xC4, 0x7F, 0x0D, 0x10 } };


/*
const
    IID_IUnknown:                       TGUID = '{00000000-0000-0000-C000-000000000046}';
    IID_IMarshal: 						TGUID = '{00000003-0000-0000-C000-000000000046}';
    IID_IRLControlPanel:                TGUID = '{60001ED3-BA0D-4115-9CF4-9AA2F8D65509}';
    IID_IRLKeyboardSink:                TGUID = '{80761278-70DA-4E77-AF64-CCC71911DF6F}';
    IID_IRLInputModule:                 TGUID = '{11AF9A3A-8C80-443D-8482-4209661FD799}';
    IID_IRLActionInput:                 TGUID = '{FBA68B07-35FF-46B2-B001-1593AC446F1F}';
    IID_IRLCustomSettings:              TGUID = '{DBDD3339-6034-4A98-B819-A320AF1033AB}';
    IID_IRLBitmapArchive:               TGUID = '{56DA887A-8888-46B0-84E8-751CA2610674}';
    IID_IRLBitmapArchiveLoader:         TGUID = '{1F04CFB4-CE66-4B24-B905-511FB6DE5DDF}';
    IID_IRLModuleLoader:                TGUID = '{3A88A0ED-080B-40F8-AACE-A8B6CC6D0D86}';
    IID_IRLUnicodeINIFileEx:            TGUID = '{E0B70F6A-403C-49C0-905C-A09D787C715F}';
    IID_IRLUnicodeINIFileExLoader:      TGUID = '{4A67B735-727E-42A9-B64C-027D6549C455}';
    IID_IKernelObjectTable:             TGUID = '{1B32463A-3F9B-47BE-8E58-5652B1610355}';
    IID_IKernelMoniker:                 TGUID = '{020712C0-D561-4468-8CDD-E2DA2C4F7F8B}';
    IID_IEnumKernelMoniker:             TGUID = '{5B6A7C36-E552-4FA9-9325-4445F0DEECE3}';
    IID_IRLInteractivityManager:        TGUID = '{13146E29-AB86-422E-89AC-290CB221C9D6}';
    IID_IRLInteractivityClient:         TGUID = '{58A4D443-21D9-485A-A8DD-715D650B4278}';
    IID_IRLEnumInteractivity:           TGUID = '{1480A01F-A1E2-4A86-8A93-AF796D4ABB5E}';
    IID_IRLExtensionList:               TGUID = '{84149B71-A545-418B-9177-7F3AB58C8E71}';
    IID_IRLWindowManager:               TGUID = '{DD3AF47E-F963-4DF0-8FDF-B20DDCA8F224}';
	IID_IRLFiltersInfo:					TGUID = '{B24EE98E-AE26-4DAD-870D-7ED4C7C5E921}';
    IID_IRLTrayIcon:                    TGUID = '{A0FF3B7C-3692-4369-A120-514E533601B4}';
	IID_IRLUserInterface:				TGUID = '{256F29B4-7C77-4F8A-970C-E1A88B14576B}';
    IID_IRLTagPlugin: 					TGUID = '{8B96F18C-3A05-4993-AC09-FF70E21CFA65}';
    IID_IRLEnumTagPlugins: 				TGUID = '{8823141C-84DC-42A7-8C3F-8C47D9195EF2}';
    IID_IRLTagFile:		   				TGUID = '{CAE7DC2C-0976-49EF-978F-7C0671680879}';
    IID_IRLTagLibraryModule:  			TGUID = '{EECFD0E6-D964-4618-83EA-0E5EC233A00C}';
    IID_IRLModuleSetInfo:				TGUID = '{78EA7CB7-96E0-401B-A494-044E2082739C}';


    CLSID_KernelObjectTable:            TGUID = '{9961BF61-0C3F-4D89-A7F8-D9EAD4F5D32C}';

	IID_ISpecifyPropertyPages : 		TGUID = '{B196B28B-BAB4-101A-B69C-00AA00341D07}';
	IID_IPropertyBag: 					TGUID = '{55272A00-42CB-11CE-8135-00AA004BB851}';
    IID_IDropTarget:					TGUID = '{00000122-0000-0000-C000-000000000046}';
*/

//-----------------------------------------------------------------------------
//
//  Message Constants
//
//-----------------------------------------------------------------------------
#define		KERNEL_SHUTDOWN					0xFFFF
#define		KERNEL_INITIALIZE               0xFF00
#define		KERNEL_COMMANDLINE              0xFF01
#define		KERNEL_MODULESETCHANGE          0xFF02	// direct order for kernel to change moduleset
#define		KERNEL_MODULESETUPDATE			0xFF03	// modulesets have been modifyied ... -> notification
#define		KERNEL_APPLICATION_ACTIVATED	0xFF04	// occurs when RL application has been activated
        
#define		RLM_ACTION                      0x0100
#define		RLM_NOTIFY                      0x0101
#define		RLM_STATUS						0x0102
#define		RLM_MENU						0x0103
#define		RLM_SKIN_ACTION					0x0104
#define		RLM_SKIN_NOTIFY					0x0105
#define		RLM_REQUEST						0x0106
#define		RLM_REQUEST_ANSWER				0x0107
#define		RLM_INIT						0x0108

#define		LANGUAGE_CHANGE                 0xFD00
#define		MLOADER_INIT                    0xFC00
#define		MODULESET_INIT                  0xFB00
#define		SETTINGS_DIRTY                  0xFE00
#define		SETTINGS_APPLY                  0xFE01
#define		INPUT_ASSIGNED                  0xFA00

//-----------------------------------------------------------------------------
#define		ACTION_KERNEL_BASE				0xFF00

#define		ACTION_SHUTDOWN                 ACTION_KERNEL_BASE + 255
#define		ACTION_MINIMIZE					ACTION_KERNEL_BASE + 254
#define     ACTION_RESTORE					ACTION_KERNEL_BASE + 253
#define     ACTION_ONTOP					ACTION_KERNEL_BASE + 252
#define     ACTION_SETTINGS_SHOW            ACTION_KERNEL_BASE + 251
#define     ACTION_SETTINGS_HIDE            ACTION_KERNEL_BASE + 250
#define     ACTION_SHOW_LOG_WINDOW			ACTION_KERNEL_BASE + 249

//-----------------------------------------------------------------------------
#define		REQUEST_KERNEL_BASE				0xFF00
#define		REQUEST_MODULE_STATUS			REQUEST_KERNEL_BASE + 255
#define	    REQUEST_MEDIAINFO_UPDATE		REQUEST_KERNEL_BASE + 240

#define		NOTIFY_MEDIAINFO_UPDATE			REQUEST_KERNEL_BASE + 240


//-----------------------------------------------------------------------------
//
//        Kernel Modes
//
//-----------------------------------------------------------------------------
#define		MODE_INACTIVE                   0
#define     MODE_MEDIA                      1
#define     MODE_DVD                        2


//-----------------------------------------------------------------------------
//
//        Run_Level Constants
//
//-----------------------------------------------------------------------------
#define		LEVEL_NOT_INITIALIZED			0
#define     LEVEL_KERNEL_INIT               1
#define     LEVEL_KERNEL_UNINIT             2
#define     LEVEL_KERNEL_PROTECTED          5
#define     LEVEL_KERNEL_READY              10

//-----------------------------------------------------------------------------
//
//        Return Values
//
//-----------------------------------------------------------------------------
#define		S_KERNEL_SHUT1					100
#define     S_KERNEL_SHUT2                  101
#define     E_OUTOFSYNC                     -100


//-----------------------------------------------------------------------------
//
//        Property Page Modes
//
//-----------------------------------------------------------------------------
#define		PAGE_NONE						0
#define     PAGE_CONTROL_PANEL				1
#define     PAGE_EXTERNAL                   2

#define     CP_WIDTH                        420
#define     CP_HEIGHT                       360

//-----------------------------------------------------------------------------
//
//        Module Categories
//
//-----------------------------------------------------------------------------
#define		CAT_USER_INTERFACE  			0x0001
#define     CAT_PLAYBACK            	    0x0002
#define     CAT_OTHER                   	0x0004
#define     CAT_ANY                         0xFFFF

//-----------------------------------------------------------------------------
//
//        Extension Categories
//
//-----------------------------------------------------------------------------
#define		EXT_CAT_VIDEO					0x0001
#define     EXT_CAT_AUDIO					0x0002
#define     EXT_CAT_SUBTITLES              	0x0004
#define     EXT_CAT_PLAYLIST				0x0008
#define     EXT_CAT_OTHER                   0xFFFF


//-----------------------------------------------------------------------------
//
//        Input Categories - default codes
//
//-----------------------------------------------------------------------------
#define		INPUTCAT_APPLICATION    		0x0001
#define     INPUTCAT_PLAYBACK           	0x0010
#define     INPUTCAT_USERINTERFACE          0x0020
#define     INPUTCAT_PLAYLIST               0x0030
#define     INPUTCAT_AUDIO                  0x0040
#define     INPUTCAT_SUBTITLES				0x0050
#define     INPUTCAT_OSD					0x0060
#define     INPUTCAT_VIDEO					0x0070
#define     INPUTCAT_USER					0xA000

//-----------------------------------------------------------------------------
//
//	Directories
//
//-----------------------------------------------------------------------------
#define     IMAGES_DIR 						L"\Resources"
#define		MODULE_DIR 						L"\Modules"
#define		UPDATE_DIR 						L"\Updates"
#define     SKINS_DIR 						L"\Skins"
#define     PLAYLIST_DIR					L"\Playlists"
#define		LANG_DIR						L"\Languages"
#define     SUBTITLE_DIR					L"\Subtitles"
#define		APP_DATA_DIR					L"\RadLight LLC\RadLight 4"
#define		TEMP_DIR						L"\Temp"


//-----------------------------------------------------------------------------
//
//        Windows Messages
//
//-----------------------------------------------------------------------------
#define		CM_CONTROLPANEL					WM_USER + 1001
#define     WM_COMMANDLINE_INCOMMING        WM_USER + 1002
#define     CM_MODULESET0                   WM_USER + 1100
#define     MAX_MENU_SETS                   100

#define     MESSAGE_SINK_PORT				0xF0F0
#define     RL_SYNCWORD						0x4C52


//-----------------------------------------------------------------------------
//
//		Subtitle Engine Flags
//
//-----------------------------------------------------------------------------
#define		SF_HIDDEN						0
#define		SF_VISIBLE						1
#define     SF_ALPHA_CONSTANT				2
#define     SF_ALPHA_LAYER					4


//-----------------------------------------------------------------------------
//
//		Encoding Modes
//
//-----------------------------------------------------------------------------
#define	    ENCODING_ANSI 					0
#define    	ENCODING_UTF8 					1
#define	    ENCODING_UNICODE				2



//-----------------------------------------------------------------------------
//
//  COM Routines
//
//-----------------------------------------------------------------------------

typedef HRESULT (__stdcall *TCreateInstanceFunc) (LPUNKNOWN Kernel, REFIID IID, LPVOID *Obj);
typedef HRESULT (__stdcall *TUninitializeFunc) ();
typedef HRESULT (__stdcall *TRegisterActionsFunc) (LPUNKNOWN ActionList);
typedef HRESULT (__stdcall *TRegisterExtensionsFunc) (LPUNKNOWN ExtensionList);
typedef HRESULT (__stdcall *TCreateInputFunc) (LPUNKNOWN ActionInput, REFIID IID, LPVOID *Obj);
typedef HRESULT (__stdcall *TCreatePluginFunc) (LPUNKNOWN Parent, REFIID IID, LPVOID *Obj);


//-----------------------------------------------------------------------------
//
//	Kernel Version Info
//
//-----------------------------------------------------------------------------
typedef struct _RLKERNELINFO {
	LPWSTR		wstrDescription;
	LPWSTR		wstrURL;
	DWORD		dwVersion;
} TRLKernelInfo, *PRLKernelInfo, * FAR LPRLKernelInfo;


//-----------------------------------------------------------------------------
//
//  TRLMessage structure
//
//  This message is to be used for all information exchange between all
//  subcomponents.
//
//
//	dwSender        		- Unique ID generated by module controller assigned
//    							to the originating module
//	dwMessage				- ID of the message
//  wParam, lParam        	- additional parameters
//  dwSize                  - size of additional data passed in pData
//  pData                   - pointer to additional data carried with the message
//
//-----------------------------------------------------------------------------
typedef struct _RLMESSAGE {
	DWORD		dwSender;
	DWORD		dwMessage;
	WPARAM		wParam;
	LPARAM		lParam;
	DWORD		dwSize;
	PBYTE		pData;
} TRLMessage, *PRLMessage, * FAR LPRLMessage;

//-----------------------------------------------------------------------------
//
//  TRLModuleInfo structure
//
//  This structure identifies each module. It provides information about
//  the module as well as initialization methods for bringing the module to
//  action.
//
//
//      wstrModuleName          - UNICODE text string - the name of the module
//      dwVersion               - 4 bytes for  xx.xx.xx.xx version numbers
//      wCategory               -
//      InitRoutine             - function creating new instance of requested
//                                module
//      UninitRoutine           - called before unloading the module from
//                                memory
//-----------------------------------------------------------------------------
typedef struct _RLMODULEINFO {
	LPWSTR					wstrModuleName;
	LPWSTR					wstrDescription;
	LPWSTR					wstrURL;
	DWORD					dwVersion;
	WORD					wCategory;
	DWORD					dwCapability;
	TCreateInstanceFunc		InitRoutine;
	TUninitializeFunc		UninitRoutine;
	TRegisterActionsFunc	RegisterActions;
	TRegisterExtensionsFunc	RegisterExtensions;
	BOOL					bPropertyPage;
} TRLModuleInfo, *PRLModuleInfo, * FAR LPRLModuleInfo;

typedef struct _RLPROPERTYPAGEINFO {
	LPWSTR					wstrPageName;
	LPWSTR					wstrDescription;
	LPWSTR					wstrURL;
	DWORD					dwVersion;
	DWORD					dwCapability;
	BYTE					bPageMode;
	TCreateInstanceFunc		InitRoutine;
	TUninitializeFunc		UninitRoutine;
} TRLPropertyPageInfo, *PRLPropertyPageInfo, * FAR LPRLPropertyPageInfo;

typedef struct _RLACTIONINFO {
	LPWSTR					wstrDefaultDescription;
	DWORD					dwLanguageID;
	WPARAM					wAction;
	WPARAM					wCategory;
} TRLActionInfo, *PRLActionInfo, * FAR LPRLActionInfo;

typedef struct _RLACTIONCATEGORY {
	LPWSTR					wstrDefaultDescription;
	DWORD					dwLanguageID;
	WPARAM					wCategory;
} TRLActionCategory, *PRLActionCategory, * FAR LPRLActionCategory;

typedef struct _RLEXTENSIONINFO {
	LPWSTR					wstrExtension;
	LPWSTR					wstrDescription;
	WORD					wCategory;
	WORD					wRegisteredBy;
} TRLExtensionInfo, *PRLExtensionInfo, * FAR LPRLExtensionInfo;

typedef struct _RLINPUTINFO {
	LPWSTR					Name;
	LPWSTR					Author;
	DWORD					Version;
	BOOL					HasPropertyPage;
	TCreateInputFunc		CreateInputFunc;
} TRLInputInfo, *PRLInputInfo, * FAR LPRLInputInfo;

typedef struct _RLWINDOWINFO {
	HWND					Handle;
	BOOL					Dockable;
	BOOL					CapturesKeyboard;
	int						Importance;

	// --- internal stuff
	RECT					RestoreRect;
	BOOL					VisibleBeforeMinimize;
	long					WndProc;
	VOID*					WndProcObj;
} TRLWindowInfo, *PRLWindowInfo, * FAR LPRLWindowInfo;

typedef struct _RLMESSAGEHEADER {
	WORD					wSyncWord;
	DWORD					dwSender;
	DWORD					dwMessage;
	WPARAM					wParam;
	LPARAM					lParam;
	DWORD					dwSize;
} TRLMessageHeader, *PRLMessageHeader, * FAR LPRLMessageHeader;

typedef struct _RLFILTERINFO {
	LPWSTR					Name;
	BOOL					HasPropertyPage;
} TRLFilterInfo, *PRLFilterInfo, * FAR LPRLFilterInfo;

typedef struct _RLLANGUAGEINFO {
	LPWSTR					LanguageName;
	LPWSTR					Author;
	char					LanguageCode[3];
	LPWSTR					Version;
} TRLLanguageInfo, *PRLLanguageInfo, * FAR LPRLLanguageInfo;

//-----------------------------------------------------------------------------
//
//	Subtitle Alpha Structure
//
//-----------------------------------------------------------------------------
typedef struct _RLSUBTITLEALPHA {
	DWORD					dwFlags;
	BYTE					bConstantAlpha;
	DWORD					dwColorKey;
	PBYTE					pbAlphaMask;
	int						iPitch;
} TRLSubtitleAlpha, *PRLSubtitleAlpha, * FAR LPRLSubtitleAlpha;


struct TMethod
{
  void *Code;
  void *Data;
};

//-----------------------------------------------------------------------------
//
//  DLL Entry Points
//
//-----------------------------------------------------------------------------
typedef HRESULT (__stdcall *TDLLGetModuleCount) (DWORD *dwCount);
typedef HRESULT (__stdcall *TDLLGetModuleInfo) (DWORD dwModule, TRLModuleInfo *Info);
typedef HRESULT (__stdcall *TDLLGetPropertyCount) (DWORD *dwCount);
typedef HRESULT (__stdcall *TDLLGetPropertyPage) (DWORD dwPage, TRLPropertyPageInfo *Page);
typedef HRESULT (__stdcall *TDLLGetVersion) (WORD *wVersion);
typedef HRESULT (__stdcall *TApplicationInitFunc) (VOID* App, HINSTANCE hInstance);
typedef HRESULT (__stdcall *TApplicationUninitFunc) ();

// TODO mozno bude treba poopravovat.. copy/paste z helpka
typedef void (__fastcall *TThreadMethod)(void);

//-----------------------------------------------------------------------------
//
//        Input Modules
//
//-----------------------------------------------------------------------------
typedef HRESULT (__stdcall *TDLLGetInputCount) (DWORD *dwCount);
typedef HRESULT (__stdcall *TDLLGetInputInfo) (DWORD dwInput, TRLInputInfo *Info);


//-----------------------------------------------------------------------------
//
//        Interfaces
//
//-----------------------------------------------------------------------------

DECLARE_INTERFACE_(IRLMessageSink, IUnknown) {
	STDMETHOD(NotifyMessage)(THIS_	TRLMessage *Msg) PURE;
};

DECLARE_INTERFACE_(IRLSettings, IRLMessageSink) {
	STDMETHOD(ReadString)(THIS_		LPWSTR pGroup, LPWSTR pItem, LPWSTR pIdent, LPWSTR *pStr) PURE;
	STDMETHOD(WriteString)(THIS_	LPWSTR pGroup, LPWSTR pItem, LPWSTR pIdent, LPWSTR pData) PURE;
	STDMETHOD(ReadInteger)(THIS_	LPWSTR pGroup, LPWSTR pItem, LPWSTR pIdent, int *Value) PURE;
	STDMETHOD(WriteInteger)(THIS_	LPWSTR pGroup, LPWSTR pItem, LPWSTR pIdent, int Value) PURE;
	STDMETHOD(ReadBool)(THIS_		LPWSTR pGroup, LPWSTR pItem, LPWSTR pIdent, BOOL *Value) PURE;
	STDMETHOD(WriteBool)(THIS_		LPWSTR pGroup, LPWSTR pItem, LPWSTR pIdent, BOOL Value) PURE;
	STDMETHOD(GetAutoSave)(THIS_	BOOL *AutoSave) PURE;
	STDMETHOD(SetAutoSave)(THIS_	BOOL AutoSave) PURE;
	STDMETHOD(ClearSection)(THIS_	LPWSTR SectionName) PURE;
};

DECLARE_INTERFACE_(IRLModule, IRLMessageSink) {
	STDMETHOD(GetModuleInfo)(THIS_	TRLModuleInfo *Info) PURE;
	STDMETHOD(SetRunLevel)(THIS_	int Level) PURE;
	STDMETHOD(Uninitialize)(THIS) PURE;
	STDMETHOD(Uninit_Status)(THIS) PURE;
	STDMETHOD(DisplayPropertyPage)(THIS) PURE;
	STDMETHOD(HidePropertyPage)(THIS) PURE;
	STDMETHOD(Active)(THIS) PURE;
	STDMETHOD(Inactive)(THIS) PURE;
	STDMETHOD(DisplayPropertyPageEx)(THIS_	int	Param) PURE;
	STDMETHOD(ActivateProVersion)(THIS) PURE;
};

DECLARE_INTERFACE_(IRLSyncMethod, IUnknown) {
	STDMETHOD(Execute)(THIS) PURE;
};

DECLARE_INTERFACE_(IRLKernel, IRLMessageSink) {
	STDMETHOD(GetKernelInfo)(THIS_	TRLKernelInfo *KernelInfo) PURE;
	STDMETHOD(GetKernelMode)(THIS_	DWORD *Mode) PURE;
	STDMETHOD(GetKernelDir)(THIS_	LPWSTR *Dir) PURE;
	STDMETHOD(GetApplication)(THIS_	VOID **App) PURE;
	STDMETHOD(GetHInstance)(THIS_	HINSTANCE *HI) PURE;
	STDMETHOD(GetKernelWindow)(THIS_	HWND *Wnd) PURE;
	STDMETHOD(SetRunLevel)(THIS_	int Level) PURE;
	STDMETHOD(AddModule)(THIS_		IRLModule *Module) PURE;
	STDMETHOD(RemoveModule)(THIS_	IRLModule *MOdule) PURE;
	STDMETHOD(RemoveAllModules)(THIS) PURE;
	STDMETHOD(Synchronize)(THIS_	TThreadMethod Method) PURE;
	STDMETHOD(Log)(THIS_	LPWSTR Msg) PURE;
	STDMETHOD(FindModule)(THIS_	TRLModuleInfo Info, IRLModule **Module) PURE;
	STDMETHOD(NotifyTask)(THIS_	LPWSTR Task) PURE;
	STDMETHOD(GetProVersionActivation)(THIS_ BOOL *Act, IRLModule **Module) PURE;
	STDMETHOD(SynchronizeEx)(THIS_	IRLSyncMethod *Method) PURE;
};

DECLARE_INTERFACE_(IRLModuleSetInfo, IUnknown) {
	STDMETHOD(ModuleSetCount)(THIS) PURE;
	STDMETHOD(GetModuleSetName)(THIS_ int i, LPWSTR *Name) PURE;
};

DECLARE_INTERFACE_(IRLEnumModules, IUnknown) {
	STDMETHOD(Reset)(THIS) PURE;
	STDMETHOD(Next)(THIS_	IRLModule **Module) PURE;
	STDMETHOD(Skip)(THIS) PURE;
};

DECLARE_INTERFACE_(IRLPropertyPage, IRLMessageSink) {
	STDMETHOD(GetPageInfo)(THIS_	TRLPropertyPageInfo *Info) PURE;
	STDMETHOD(SetRunLevel)(THIS_	int Level) PURE;
	STDMETHOD(DisplayWindow)(THIS_	HWND Parent, RECT *pClientRect) PURE;
	STDMETHOD(GetCaption)(THIS_	LPWSTR *Caption) PURE;
	STDMETHOD(Uninitialize)(THIS) PURE;
	STDMETHOD(Uninit_Status)(THIS) PURE;
};

DECLARE_INTERFACE_(IRLControlPanel, IRLMessageSink) {
	STDMETHOD(SetRunLevel)(THIS_	int Level) PURE;
	STDMETHOD(AddPage)(THIS_	IRLPropertyPage *Page, BOOL Dyn) PURE;
	STDMETHOD(RemovePage)(THIS_	IRLPropertyPage *Page) PURE;
	STDMETHOD(RemoveAllPages)(THIS) PURE;
};

DECLARE_INTERFACE_(IRLLanguage, IRLMessageSink) {
	STDMETHOD(GetString)(THIS_	LPWSTR Module, LPWSTR ID, LPWSTR *Value, LPWSTR Default) PURE;
	STDMETHOD(GetInfo)(THIS_ TRLLanguageInfo *Info) PURE;
};

DECLARE_INTERFACE_(IRLActionList, IUnknown) {
	STDMETHOD(AddItem)(THIS_	PRLActionInfo pItem) PURE;
	STDMETHOD(RemoveLast)(THIS) PURE;
	STDMETHOD(Count)(THIS) PURE;
	STDMETHOD(GetItem)(THIS_	DWORD dwItem, TRLActionInfo *Item) PURE;
	STDMETHOD(CategoryCount)(THIS) PURE;
	STDMETHOD(RegisterCategory)(THIS_ TRLActionCategory *pCategory) PURE;
	STDMETHOD(GetCategory)(THIS_	DWORD dwCar, TRLActionCategory *Cat) PURE;
	STDMETHOD(FindCategory)(THIS_	WPARAM wCategory, TRLActionCategory *Cat) PURE;
};

DECLARE_INTERFACE_(IRLKeyboardSink, IUnknown) {
	STDMETHOD(KeyDown)(THIS_	WPARAM wParam, LPARAM lParam) PURE;
	STDMETHOD(KeyDownEx)(THIS_	WPARAM wParam, LPARAM lParam, BOOL Global) PURE;
};

DECLARE_INTERFACE_(IRLInputModule, IUnknown) {
	STDMETHOD(GetInfo)(THIS_	TRLInputInfo *Info) PURE;
	STDMETHOD(Uninitialize)(THIS) PURE;
	STDMETHOD(DisplayPropertyPage)(THIS) PURE;
	STDMETHOD(HidePropertyPage)(THIS) PURE;
};

DECLARE_INTERFACE_(IRLActionInput, IUnknown) {
	STDMETHOD(NotifyInput)(THIS_	LPWSTR Module, LPWSTR InputFriendlyName) PURE;
	STDMETHOD(GetAction)(THIS_		LPWSTR Module, WORD Action, LPWSTR *Value) PURE;
	STDMETHOD(SetAction)(THIS_		LPWSTR Module, WORD Action, LPWSTR Value) PURE;
	STDMETHOD(SetAssigning)(THIS_	LPWSTR Module, WORD Action) PURE;
	STDMETHOD(GetInputCount)(THIS_	DWORD *dwCount) PURE;
	STDMETHOD(GetInput)(THIS_		DWORD Index, IRLInputModule **Module) PURE;
	STDMETHOD(GetKernel)(THIS_		IRLKernel **Kernel) PURE;
	STDMETHOD(SaveSettings)(THIS) PURE;
	STDMETHOD(LoadSettings)(THIS) PURE;
	STDMETHOD(GetEnabled)(THIS_		LPWSTR Module, BOOL *Enabled) PURE;
	STDMETHOD(SetEnabled)(THIS_		LPWSTR Module, BOOL Enabled) PURE;
	STDMETHOD(NotifyInputEx)(THIS_	LPWSTR Module, LPWSTR InputFriendlyName, BOOL Global) PURE;
	STDMETHOD(GetGlobal)(THIS_		LPWSTR Module, WORD Action, BOOL *Global) PURE;
	STDMETHOD(SetGlobal)(THIS_		LPWSTR Module, WORD Action, BOOL Global) PURE;
};

DECLARE_INTERFACE_(IRLCustomSettings, IUnknown) {
	STDMETHOD(ImportSettings)(THIS) PURE;
	STDMETHOD(ExportSettings)(THIS) PURE;
};

DECLARE_INTERFACE_(IRLBitmapArchive, IUnknown) {
	STDMETHOD(GetBitmapCount)(THIS_	DWORD *Count) PURE;
	STDMETHOD(GetBitmapInfo)(THIS_	DWORD Index, int *Width, int *Height, char **ResName) PURE;
	STDMETHOD(GetResInfo)(THIS_		char *ResName, int *Width, int *Height) PURE;
	STDMETHOD(LoadBitmap)(THIS_		char *ResName, HDC DC) PURE;
};

DECLARE_INTERFACE_(IRLBitmapArchiveLoader, IUnknown) {
	STDMETHOD(LoadArchive)(THIS_	LPWSTR FileName, IRLBitmapArchive **Archive) PURE;
};

DECLARE_INTERFACE_(IRLModuleLoader, IUnknown) {
	STDMETHOD(GetAvailableModuleCount)(THIS_	DWORD *Count) PURE;
	STDMETHOD(GetModuleInfo)(THIS_				DWORD Index, TRLModuleInfo *Info) PURE;
	STDMETHOD(GetAvailablePropertyPageCount)(THIS_ DWORD *Count) PURE;
	STDMETHOD(GetPropertyPageInfo)(THIS_	DWORD Index, TRLPropertyPageInfo *Info) PURE;
	STDMETHOD(LocateModule)(THIS_			TRLModuleInfo Info, LPWSTR *FileName) PURE;
	STDMETHOD(LocatePropertyPage)(THIS_		TRLPropertyPageInfo Info, LPWSTR *FileName) PURE;
};

DECLARE_INTERFACE_(IRLInteractivityClient, IUnknown) {
	STDMETHOD(NotifyVideoDimensions)(THIS_	int Width, int Height) PURE;
	STDMETHOD(NotifyMousePos)(THIS_			int X, int Y) PURE;
	STDMETHOD(NotifyMouseDown)(THIS_		int X, int Y, int Button, int *Action) PURE;
	STDMETHOD(NotifyMouseUp)(THIS_			int X, int Y) PURE;
};

DECLARE_INTERFACE_(IRLEnumInteractivity, IUnknown) {
	STDMETHOD(Reset)(THIS) PURE;
	STDMETHOD(Next)(THIS_ IRLInteractivityClient **Client) PURE;
	STDMETHOD(Skip)(THIS) PURE;
};

DECLARE_INTERFACE_(IRLInteractivityManager, IUnknown) {
	STDMETHOD(EnumClients)(THIS_	IRLEnumInteractivity **Enum) PURE;
	STDMETHOD(RegisterClient)(THIS_	IRLInteractivityClient *Client) PURE;
	STDMETHOD(RevokeClient)(THIS_	IRLInteractivityClient *Client) PURE;
	STDMETHOD(RevokeAll)(THIS) PURE;
	STDMETHOD(NotifyMousePos)(THIS_	int X, int Y) PURE;
	STDMETHOD(NotifyMouseDown)(THIS_ int X, int Y, int Button, int *Action) PURE;
	STDMETHOD(NotifyMouseUp)(THIS_	int X, int Y) PURE;
	STDMETHOD(NotifyVideoDimensions)(THIS_ int Width, int Height) PURE;
};

DECLARE_INTERFACE_(IRLExtensionList, IUnknown) {
	STDMETHOD(AddItem)(THIS_	PRLExtensionInfo pItem) PURE;
	STDMETHOD(RemoveLast)(THIS) PURE;
	STDMETHOD(Count)(THIS) PURE;
	STDMETHOD(GetItem)(THIS_	DWORD dwItem, TRLExtensionInfo *Item) PURE;
	STDMETHOD(CheckExtension)(THIS_	LPWSTR wstrExt) PURE;
	STDMETHOD(GetCategoryFilter)(THIS_	WORD Cat, LPWSTR *wstrFilter) PURE;
	STDMETHOD(CheckExtensionEx)(THIS_	LPWSTR wstrExt, WORD Cat) PURE;
	STDMETHOD(RemoveRegisteredBy)(THIS_	DWORD dwRegisteredBy) PURE;
};

DECLARE_INTERFACE_(IRLWindowManager, IUnknown) {
	STDMETHOD(Minimize)(THIS) PURE;
	STDMETHOD(Restore)(THIS) PURE;
	STDMETHOD(Activate)(THIS) PURE;
	STDMETHOD_(BOOL, IsActive)(THIS) PURE;
	STDMETHOD(AddWindow)(THIS_ TRLWindowInfo Info) PURE;
	STDMETHOD(RemoveWindow)(THIS_ HWND Wnd) PURE;
	STDMETHOD(GetKeyboardCapture)(THIS_	HWND Wnd) PURE;
};

DECLARE_INTERFACE_(IRLTrayIcon, IUnknown) {
	STDMETHOD(ShowBaloonHint)(THIS_ LPWSTR wstrTitle, LPWSTR wstrValue, DWORD dwBaloonType, DWORD dwDelay) PURE;
	STDMETHOD(GetHint)(THIS_	LPWSTR *Hint) PURE;
	STDMETHOD(SetHint)(THIS_	LPWSTR Hint) PURE;
};

DECLARE_INTERFACE_(IRLUserInterface, IUnknown) {
	STDMETHOD(LoadSkin)(THIS_	LPWSTR SkinFile, LPWSTR Mode, LPWSTR ColorTheme) PURE;
};


#define CheckPointer(p, ret)		if ((p) == NULL) return (ret)

/* Return an interface pointer to a requesting client
   performing a thread safe AddRef as necessary */
STDAPI GetInterface(LPUNKNOWN pUnk, void **ppv);

HRESULT GetModuleInterface(IRLKernel *Kernel, REFIID riid, void **ppv);


#if (_MSC_VER <= 1200)
#pragma warning(disable:4211)

/* The standard InterlockedXXX functions won't take volatiles */
static inline LONG WINAPI InterlockedIncrement( volatile LONG * plong )
{ return InterlockedIncrement( const_cast<LONG*>( plong ) ); }

static inline LONG WINAPI InterlockedDecrement( volatile LONG * plong )
{ return InterlockedDecrement( const_cast<LONG*>( plong ) ); }

#pragma warning(default:4211)
#endif

#define INTERFACE_RELEASE(x)	if	((x)) { (x)->Release(); (x) = NULL; } 


//-----------------------------------------------------------------------------
//
//  TBaseObject Class
//
//  Supports Interface deriving. But does not keep the eye on reference count
//
//-----------------------------------------------------------------------------
class TBaseObject : public IUnknown
{
public:
    /* Iunknown implementation */
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();
};

//-----------------------------------------------------------------------------
//
//  TRLComObject Class
//
//  C++ CUnknown Clone.
//
//-----------------------------------------------------------------------------
class TRLComObject : public IUnknown
{
private:
	LONG		m_iRefCount;
public:
	TRLComObject();
	virtual ~TRLComObject();

    /* Iunknown implementation */
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);
};

//-----------------------------------------------------------------------------
//
//	TSyncMethod Class
//
//	Class for C++ synchronize calls.... a bit clumsy....
//
//-----------------------------------------------------------------------------
class TSyncMethod : public TRLComObject,
					public IRLSyncMethod
{
public:
	// --- IUnknown overriden
    STDMETHODIMP QueryInterface(REFIID riid, void **ppv);
	STDMETHODIMP_(ULONG) AddRef(void) { return TRLComObject::AddRef(); }
	STDMETHODIMP_(ULONG) Release(void) { return TRLComObject::Release(); }

	// --- IRLSyncMethod method
	STDMETHODIMP Execute() { return NOERROR; }
};

//-----------------------------------------------------------------------------
//
//  TLock Class
//
//-----------------------------------------------------------------------------
class TLock
{
private:
	CRITICAL_SECTION	m_CritSec;
public:
	TLock() { InitializeCriticalSection(&m_CritSec); }
	~TLock() { DeleteCriticalSection(&m_CritSec); }
	void Lock() { EnterCriticalSection(&m_CritSec); }
	void Unlock() { LeaveCriticalSection(&m_CritSec); }
};

//-----------------------------------------------------------------------------
//
//  TAutoLock Class
//
//-----------------------------------------------------------------------------
class TAutoLock
{
private:
	TLock		*m_pLock;
public:
	TAutoLock(TLock *pLock) : m_pLock(pLock) { m_pLock->Lock(); }
	~TAutoLock() { m_pLock->Unlock(); }
};

//-----------------------------------------------------------------------------
//
//  TEvent Class
//
//-----------------------------------------------------------------------------
class TEvent
{
private:
	HANDLE		m_hEvent;
public:
	TEvent(BOOL fManualReset = FALSE);
	~TEvent();
	
	void SetEv() { SetEvent(m_hEvent); }
	void ResetEv() { ResetEvent(m_hEvent); }
	BOOL Check() { return Wait(0); }
	BOOL Wait(DWORD dwTimeout = INFINITE);

	// read-only property
	HANDLE Handle() { return m_hEvent; }
};


//-----------------------------------------------------------------------------
//
//	TWorkerThread Class
//
//-----------------------------------------------------------------------------
class TWorkerThread
{
private:
	TEvent			m_ReplyEvent;
	TEvent			m_CommandRequest;
	HRESULT			m_Reply;
	int				m_iCommand;
	
	TLock			m_AccessLock;
	TLock			m_WorkerLock;
	HANDLE			m_hThread;

public:
	TWorkerThread();
	~TWorkerThread();
	
	// create / kill thread
	BOOL CreateThread();
	void CloseThread();
	BOOL ThreadExists();

	// thread proc
	virtual DWORD ThreadProc() { return NOERROR; }
    static DWORD WINAPI GlobalThreadProc(void *pParam);

	HRESULT CallWorker(int Cmd);
	void Reply(HRESULT Value);
	int GetCommand();
	BOOL CheckRequest();
	
	virtual void OnInit() { }
	virtual void OnExit() { }

	// use this lock for internal locking in derived classes
	TLock *Lock() { return &m_WorkerLock; }
	HANDLE Handle() { return m_hThread; }
};

#define	CMD_INIT			0
#define CMD_RUN				1
#define CMD_STOP			2
#define	CMD_EXIT			3
#define CMD_ANALZYE			4
#define CMD_SCAN			5




#endif /* URLTYPES_H */
