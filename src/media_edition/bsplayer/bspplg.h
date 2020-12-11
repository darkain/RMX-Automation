#ifndef _BSPPLG_H
#define _BSPPLG_H

typedef struct pluginInfo {
  char description[256];//Filled in by plugin
  int (__stdcall *plgCallback)(const DWORD cmdID,void * param1,void *param2);//Filled in by plugin

  HWND hwndParent;//Filled in by BSPlayer
  HWND hwndVideoWin;//Filled in by BSPlayer

  void (__stdcall *execAction)(const int a_action);//Filled in by BSPlayer
 
  // Register additional extension for subtitles
  //  subRegisterExt("*.mysub1;*.mysub2");
  //  for every registered extension function plgCallback will be called with cmdID EV_LOAD_SUB and param1 as file name
  void (__stdcall *subRegisterExt)(const char *ext);//Filled in by BSPlayer
  int (__stdcall *createSubs)(const char *subName);//Filled in by BSPlayer
  DWORD (__stdcall *addLine)(const DWORD subID,DWORD substart,DWORD substop,char *subLine);//Filled in by BSPlayer
  void (__stdcall *activateSubs)(const int subID);//Filled in by BSPlayer

  //OSD functions
  // showTime - How long should OSD be displayed
  // OSDid - must be >=50000 or 0
  void (__stdcall *ShowOSDText)(const char *osdText,const DWORD showTime,const DWORD OSDid);//Filled in by BSPlayer

} pluginInfo,*PpluginInfo;


//events
#define  EV_UNLOAD              40000 //plugin is unloading
#define  EV_BEFORE_FILE_OPEN    40001 //Called before file is loaded, param1 - pointer to filename
#define  EV_FILE_OPEN           40002 //Called after file is loaded (movie is open), param1 - pointer to filename
#define  EV_CONFIG              40003 //param1 - window handle
#define  EV_ABOUT               40004 //param1 - window handle
#define  EV_LOAD_SUB            40005 //param1 - file name and path to subtitle file
#define  EV_PLAY                40006
#define  EV_STOP                40007
//#define  EV_OSD                 40008 //Called on every OSD message, param1 - pointer to OSD text

  

#endif //_BSPPLG_H