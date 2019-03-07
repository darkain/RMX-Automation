//-----------------------------------------------------------------------------
//
//  RadLight 4.0 Source
//
//  Playback Interfaces
//
//  Author : Igor Janos
//  Last Update : 2-jul-2003
//
//-----------------------------------------------------------------------------

#ifndef URLPLAYBACK_H
#define URLPLAYBACK_H



//-----------------------------------------------------------------------------
//
//	Action Codes
//
//-----------------------------------------------------------------------------
#define	ACTION_PLAYBACK_BASE				0x0A00

#define	ACTION_PLAYBACK_PLAY				ACTION_PLAYBACK_BASE + 1
#define	ACTION_PLAYBACK_STOP				ACTION_PLAYBACK_BASE + 2
#define	ACTION_PLAYBACK_PAUSE				ACTION_PLAYBACK_BASE + 3
#define	ACTION_PLAYBACK_CLOSE				ACTION_PLAYBACK_BASE + 4
#define	ACTION_PLAYBACK_SEEKFW				ACTION_PLAYBACK_BASE + 5
#define	ACTION_PLAYBACK_SEEKBW	 			ACTION_PLAYBACK_BASE + 6
#define	ACTION_PLAYBACK_VOLUP				ACTION_PLAYBACK_BASE + 7
#define	ACTION_PLAYBACK_VOLDOWN	   			ACTION_PLAYBACK_BASE + 8
#define	ACTION_PLAYBACK_OPEN				ACTION_PLAYBACK_BASE + 9
#define	ACTION_PLAYBACK_PLAY_PAGE			ACTION_PLAYBACK_BASE + 10
#define	ACTION_PLAYBACK_MUTE				ACTION_PLAYBACK_BASE + 11
#define	ACTION_PLAYBACK_VOLUME_MAJ_UP		ACTION_PLAYBACK_BASE + 12
#define	ACTION_PLAYBACK_VOLUME_MAJ_DOWN		ACTION_PLAYBACK_BASE + 13
#define	ACTION_PLAYBACK_TOGGLE_CROSSFADE	ACTION_PLAYBACK_BASE + 14
#define	ACTION_PLAYBACK_CYCLE_AUDIOSTREAM 	ACTION_PLAYBACK_BASE + 15
#define	ACTION_PLAYBACK_STREAM_CAPTURE		ACTION_PLAYBACK_BASE + 16

#define	ACTION_PLAYBACK_OBJECT_RELEASE		ACTION_PLAYBACK_BASE + 0x0080;


//-----------------------------------------------------------------------------
//
//	Message Constants
//
//-----------------------------------------------------------------------------
#define	RLM_SEEK_NOTIFY						0xFA10

#define	NOTIFY_PLAYBACK_BASE				ACTION_PLAYBACK_BASE
    
#define	NOTIFY_PLAYBACK_FILE_OPENING		NOTIFY_PLAYBACK_BASE + 1
#define	NOTIFY_PLAYBACK_FILE_OPENED			NOTIFY_PLAYBACK_BASE + 2
#define NOTIFY_PLAYBACK_FILE_CLOSED			NOTIFY_PLAYBACK_BASE + 3
#define NOTIFY_PLAYBACK_FILE_OPEN_ERROR		NOTIFY_PLAYBACK_BASE + 4
#define NOTIFY_PLAYBACK_PLAYING				NOTIFY_PLAYBACK_BASE + 5
#define NOTIFY_PLAYBACK_STOPPED				NOTIFY_PLAYBACK_BASE + 6
#define NOTIFY_PLAYBACK_PAUSED				NOTIFY_PLAYBACK_BASE + 7
#define NOTIFY_PLAYBACK_COMPLETE			NOTIFY_PLAYBACK_BASE + 8
#define NOTIFY_PLAYBACK_USER_ABORT			NOTIFY_PLAYBACK_BASE + 9
#define NOTIFY_PLAYBACK_SEEKING				NOTIFY_PLAYBACK_BASE + 10

//-----------------------------------------------------------------------------
//
//	Notify Codes
//
//-----------------------------------------------------------------------------
#define	NOTIFY_PLAYBACK_RATE				NOTIFY_PLAYBACK_BASE + 11
#define	NOTIFY_PLAYBACK_VOLUME				NOTIFY_PLAYBACK_BASE + 12
#define	NOTIFY_PLAYBACK_BALANCE				NOTIFY_PLAYBACK_BASE + 13
#define	NOTIFY_PLAYBACK_MUTE				NOTIFY_PLAYBACK_BASE + 14
#define	NOTIFY_PLAYBACK_CHAPTER_UPDATE		NOTIFY_PLAYBACK_BASE + 15
#define	NOTIFY_PLAYBACK_BOOKMARK_UPDATE		NOTIFY_PLAYBACK_BASE + 20
#define	NOTIFY_PLAYBACK_CROSSFADE			NOTIFY_PLAYBACK_BASE + 21
#define	NOTIFY_PLAYBACK_CLOSING_VIDEO		NOTIFY_PLAYBACK_BASE + 22
#define	NOTIFY_PLAYBACK_NO_MEDIA_LOADED 	NOTIFY_PLAYBACK_BASE + 23
#define	NOTIFY_PLAYBACK_FILTERS_CHANGED		NOTIFY_PLAYBACK_BASE + 24

#define	REQUEST_BUFFER_LEVEL				ACTION_PLAYBACK_BASE + 1


//-----------------------------------------------------------------------------
//
//	GUIDS
//
//-----------------------------------------------------------------------------


// {6B8276EB-C51A-4ACE-8082-45C2F6EF622D}
static const GUID IID_IRLPlayback = 
{ 0x6B8276EB, 0xC51A, 0x4ACE, { 0x80, 0x82, 0x45, 0xC2, 0xF6, 0xEF, 0x62, 0x2D } };

// {A2112E3B-948A-440F-AC52-466B0B9C73BD}
static const GUID IID_IRLPlayback2 = 
{ 0xA2112E3B, 0x948A, 0x440F, { 0xAC, 0x52, 0x46, 0x6B, 0x0B, 0x9C, 0x73, 0xBD } };

// {7B9E1EB3-093E-4893-950B-C561B5D2C5DF}
static const GUID IID_IRLVideoWindow = 
{ 0x7B9E1EB3, 0x093E, 0x4893, { 0x95, 0x0B, 0xC5, 0x61, 0xB5, 0xD2, 0xC5, 0xDF } };

// {B24EE98E-AE26-4DAD-870D-7ED4C7C5E921}
static const GUID IID_IRLFiltersInfo = 
{ 0xB24EE98E, 0xAE26, 0x4DAD, { 0x87, 0x0D, 0x7E, 0xD4, 0xC7, 0xC5, 0xE9, 0x21 } };

// {5C151330-F6E8-44B2-B9A3-36792156615F}
static const GUID IID_IRLMediaInfo = 
{ 0x5C151330, 0xF6E8, 0x44B2, { 0xB9, 0xA3, 0x36, 0x79, 0x21, 0x56, 0x61, 0x5F } };


//-----------------------------------------------------------------------------
//
//	Structures
//
//-----------------------------------------------------------------------------

typedef struct _TChapterInfo {
	double		Time;
	LPWSTR		Text;
	HBITMAP		Image;
} TChapterInfo, *PChapterInfo, *FAR LPChapterInfo;

typedef struct _TBookmarkInfo {
	double		Time;
	LPWSTR		Text;
	HBITMAP		Image;
} TBookmarkInfo, *PBookmarkInfo, *FAR LPBookmarkInfo;

typedef struct _TSubtitleInfo {
	LPWSTR		Text;			// subtitle ID
	LPWSTR		URL;			// either the URL or Embedded info.
} TSubtitleInfo, *PSubtitleInfo, *FAR LPSubtitleInfo;


//-----------------------------------------------------------------------------
//
//	Interfaces
//
//-----------------------------------------------------------------------------

DECLARE_INTERFACE_(IRLPlayback, IRLMessageSink) {
	STDMETHOD(QueryGraphBuilder)(THIS_ REFIID riid, void **ppv) PURE;
	STDMETHOD(OpenFile)(THIS_ LPWSTR FileName, DWORD ID) PURE;
	STDMETHOD(OpenStream)(THIS_ LPWSTR OpenStr, DWORD ID) PURE;
	STDMETHOD(Close)(THIS) PURE;
	STDMETHOD(Play)(THIS) PURE;
	STDMETHOD(Stop)(THIS) PURE;
	STDMETHOD(Pause)(THIS) PURE;
	STDMETHOD(GetDuration)(THIS_	double *Dur) PURE;
	STDMETHOD(GetCurrentPosition)(THIS_ double *Pos) PURE;
	STDMETHOD(SetCurrentPosition)(THIS_ double Pos) PURE;
	STDMETHOD(GetRate)(THIS_ double *Rate) PURE;
	STDMETHOD(SetRate)(THIS_ double Rate) PURE;
	STDMETHOD(GetVolume)(THIS_ double *Volume) PURE;
	STDMETHOD(SetVolume)(THIS_ double Volume) PURE;
	STDMETHOD(GetCurrentURL)(THIS_ LPWSTR *URL) PURE;
	STDMETHOD(GetMute)(THIS_ BOOL *Mute) PURE;
	STDMETHOD(SetMute)(THIS_ BOOL Mute) PURE;
	STDMETHOD(GetBalance)(THIS_ double *Balance) PURE;
	STDMETHOD(SetBalance)(THIS_ double Balance) PURE;
	STDMETHOD(GetCrossfade)(THIS_ BOOL *Crossfade) PURE;
	STDMETHOD(SetCrossfade)(THIS_ BOOL Crossfade) PURE;
	STDMETHOD_(BOOL, CanSeek)(THIS) PURE;
	STDMETHOD_(BOOL, CanPlayrate)(THIS) PURE;
};

DECLARE_INTERFACE_(IRLPlayback2, IRLPlayback) {
	STDMETHOD(OpenFileAtTime)(THIS_ LPWSTR FileName, DWORD ID, double Time) PURE;
//TODO:	STDMETHOD(OpenStreamSource)(THIS_ LPWSTR URL, IStreamBase *Source, PBYTE MediaType, DWORD ID) PURE;
};

DECLARE_INTERFACE_(IRLVideoWindow, IRLMessageSink) {
	STDMETHOD(IsVideoPresent)(THIS) PURE;
	STDMETHOD(GetVideoSize)(THIS_ int *Width, int *Height) PURE;
	STDMETHOD(GetVisible)(THIS_ BOOL *Visible) PURE;
	STDMETHOD(SetVisible)(THIS_ BOOL Visible) PURE;
	STDMETHOD(GetParent)(THIS_ HWND *Parent) PURE;
	STDMETHOD(SetParent)(THIS_ HWND Parent) PURE;
	STDMETHOD(GetBounds)(THIS_ int *Left, int *Top, int *Width, int *Height) PURE;
	STDMETHOD(SetBounds)(THIS_ int Left, int Top, int Width, int Height) PURE;
	STDMETHOD(SetMessageDrain)(THIS_ HWND Drain) PURE;
	STDMETHOD(NotifyWindow)(THIS_ UINT uMsg, WPARAM wParam, LPARAM lParam) PURE;
};

DECLARE_INTERFACE_(IRLFiltersInfo, IRLMessageSink) {
	STDMETHOD(GetFilterCount)(THIS_ int *Count) PURE;
	STDMETHOD(GetFilter)(THIS_ int i, TRLFilterInfo *Info) PURE;
	STDMETHOD(ShowPropertyPage)(THIS_ LPWSTR Name) PURE;
};








#endif /* URLPLAYBACK_H */