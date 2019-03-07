////////////////////////////////////////////////////////////////////
// BSPlayer API v 1.1
//
// To get BSPlayer window handle use:
// bsp_hand=FindWindow("BSPlayer",NULL);
//
// then you can send messages, for ex. to get current version:
// bsp_ver = SendMessage(bsp_hand,WM_BSP_CMD,BSP_GETVERSION,0);
////////////////////////////////////////////////////////////////////
#ifndef _BSP_H
#define _BSP_H


#define WM_BSP_CMD WM_USER+2


// bsp_ver = SendMessage(bsp_hand,WM_BSP_CMD,BSP_GETVERSION,0);
// available in BSPlayer version 0.84.484+
//
// Return player version
// format will be XX.YY.ZZZZ -> 0xXXYYZZZZ
// for 0.84.484 -> 0x00840484
#define BSP_GETVERSION 0x10000

// mov_len = SendMessage(bsp_hand,WM_BSP_CMD,BSP_GetMovLen,0);
// available in BSPlayer version 0.84.484+
//
// Return moive length in msec
#define BSP_GetMovLen  0x10100

// mov_pos = SendMessage(bsp_hand,WM_BSP_CMD,BSP_GetMovPos,0);
// available in BSPlayer version 0.84.484+
//
// Return current movie position in msec
#define BSP_GetMovPos  0x10101

// status = SendMessage(bsp_hand,WM_BSP_CMD,BSP_GetStatus,0);
// available in BSPlayer version 0.84.484+
//
// Return player status
// 0 - STOP
// 1 - PAUSE
// 2 - PLAY
// 4 - No movie open
#define BSP_GetStatus  0x10102

// SendMessage(bsp_hand,WM_BSP_CMD,BSP_Seek,t);
// t is time in msec
// available in BSPlayer version 0.84.484+
//
// Seek to specified position
#define BSP_Seek       0x10103


// SendMessage(bsp_hand,WM_BSP_CMD,BSP_SetVol,vol);
// vol is volume in range 0-24
// available in BSPlayer version 0.84.484+
//
// Set volume
#define BSP_SetVol     0x10104

// vol = SendMessage(bsp_hand,WM_BSP_CMD,BSP_GetVol,0);
// available in BSPlayer version 0.84.484+
//
//Get volume
#define BSP_GetVol     0x10105



// v1.00+
// all this functions with WM_COPYDATA message can be called from plugins like this:
//
// char buf[MAX_PATH];
// SendMessage(bsp_hand,WM_BSP_CMD,BSP_GetSkin,buf);
//
// SendMessage(bsp_hand,WM_BSP_CMD,BSP_SetSkin,"SkinName");


// COPYDATASTRUCT cds;
//
// cds.dwData=BSP_SetSkin;
// cds.lpData=(void *) "skin_name";
// cds.cbData=strlen((char *)cds.lpData)+1;
// SendMessage(bsp_hand,WM_COPYDATA,0,(LPARAM)&cds);
// available in BSPlayer version 0.84.484+
//
// Set new skin
#define BSP_SetSkin    0x10106

// COPYDATASTRUCT cds;
// char buf[MAX_PATH];
// void * adr;
//
// adr=&buf;
// cds.dwData=BSP_GetSkin;
// cds.lpData=&adr;
// cds.cbData=4;
// SendMessage(bsp_hand,WM_COPYDATA,appHWND,(LPARAM)&cds);
// available in BSPlayer version 0.84.484+
//
// appHWND is calling application window handle
//
// Get current skin, skin name will be copied to buf
//
#define BSP_GetSkin    0x10107



// COPYDATASTRUCT cds;
//
// cds.dwData=BSP_OpenFile;
// cds.lpData=(void *) "file.avi";
// cds.cbData=strlen((char *)cds.lpData)+1;
// SendMessage(bsp_hand,WM_COPYDATA,0,(LPARAM)&cds);
// available in BSPlayer version 0.84.484+
//
// Open file
#define BSP_OpenFile    0x10108

// COPYDATASTRUCT cds;
//
// cds.dwData=BSP_LoadSub;
// cds.lpData=(void *) "subtitles.sub";
// cds.cbData=strlen((char *)cds.lpData)+1;
// SendMessage(bsp_hand,WM_COPYDATA,0,(LPARAM)&cds);
// available in BSPlayer version 0.84.484+
//
// Load subtitles
#define BSP_LoadSub    0x10109

// COPYDATASTRUCT cds;
//
// cds.dwData=BSP_LoadAudio;
// cds.lpData=(void *) "file.mp3";
// cds.cbData=strlen(((char *)cds.lpData)+1;
// SendMessage(bsp_hand,WM_COPYDATA,0,(LPARAM)&cds);
// available in BSPlayer version 0.84.484+
//
// Load external audio file
#define BSP_LoadAudio    0x1010A

// COPYDATASTRUCT cds;
// char buf[MAX_PATH];
// void *adr;
//
// adr=&buf;
// cds.dwData=BSP_GetFileName;
// cds.lpData=&adr;
// cds.cbData=4;
// SendMessage(bsp_hand,WM_COPYDATA,appHWND,(LPARAM)&cds);
// available in BSPlayer version 0.84.484+
//
// appHWND is calling application window handle
// File name will be copied to buf
//
// Get open file name
#define BSP_GetFileName  0x1010B

// COPYDATASTRUCT cds;
//
// cds.dwData=BSP_LoadPlaylist;
// cds.lpData=(void *) "file.bsl";
// cds.cbData=strlen(((char *)cds.lpData)+1;
// SendMessage(bsp_hand,WM_COPYDATA,0,(LPARAM)&cds);
// available in BSPlayer version 1.00.800+
//
// Load playlist
#define BSP_LoadPlaylist    0x1010C

// COPYDATASTRUCT cds;
//
// cds.dwData=BSP_LoadPlaylistInt;
// cds.lpData=(void *) "file.bls";
// cds.cbData=strlen(((char *)cds.lpData)+1;
// SendMessage(bsp_hand,WM_COPYDATA,0,(LPARAM)&cds);
// available in BSPlayer version 1.00.800+
//
// Load playlist in internal format
#define BSP_LoadPlaylistInt    0x1010D

// Other commands

#define BSP_ExitFScreen 0
#define BSP_VolUp 1
#define BSP_VolDown 2
#define BSP_DeDynUp 3
#define BSP_DeDynPreUp 4
#define BSP_DeDynDown 5
#define BSP_DeDynPreDown 6
#define BSP_Preferences 7
#define BSP_FrmCapture 8
#define BSP_Frm2 9
#define BSP_FS_Switch 10
#define BSP_SubsEnDi 11
#define BSP_Skins 12
#define BSP_AStrmVolCyc 13
#define BSP_Rew 14
#define BSP_Forw 15
#define BSP_SubCorInc 16
#define BSP_SubCorDec 17
#define BSP_SubCorIncS 18
#define BSP_SubCorDecS 19
#define BSP_Play 20
#define BSP_Pause 21
#define BSP_Stop 22
#define BSP_ViewChp 23
#define BSP_VBlankSwitch 24
#define BSP_Prev 25
#define BSP_PrevCh 26
#define BSP_PrevCD 27
#define BSP_Next 28
#define BSP_NextCh 29
#define BSP_NextCD 30
#define BSP_ATop 31
#define BSP_OvrTop 32
#define BSP_AspCyc 33
#define BSP_PlayList 34
#define BSP_Mute 35
#define BSP_JumpToTime 36
#define BSP_Zoom50 37
#define BSP_Zoom100 38
#define BSP_Zoom200 39
#define BSP_AspOrg 40
#define BSP_Asp169 41
#define BSP_Asp43 42
#define BSP_FSSW640 43
#define BSP_FSSW800 44
#define BSP_VInf 45
#define BSP_PanIn 46
#define BSP_PanOut 47
#define BSP_ZoomIn 48
#define BSP_ZoomOut 49
#define BSP_MoveLeft 50
#define BSP_MoveRight 51
#define BSP_MoveUp 52
#define BSP_MoveDown 53
#define BSP_FRSizeLeft 54
#define BSP_FRSizeRight 55
#define BSP_FRSizeUp 56
#define BSP_FRSizeDown 57
#define BSP_ResetMov 58
#define BSP_HideCtrl 59
#define BSP_EQ 60
#define BSP_OpenAud 61
#define BSP_OpenSub 62
#define BSP_OpenMov 63
#define BSP_PanScan 64
#define BSP_CusPanScan 65
#define BSP_DeskMode 66
#define BSP_AddBk 67
#define BSP_EditBK 68
#define BSP_SkinRefr 69
#define BSP_About 70
#define BSP_CycleAS 71
#define BSP_CycleSub 72
#define BSP_IncPBRate 73
#define BSP_DecPBRate 74
#define BSP_IncPP 75
#define BSP_DecPP 76
#define BSP_Exit 77
#define BSP_CloseM 78
#define BSP_JumpF 79
#define BSP_JumpB 80
#define BSP_ChBordEx 81
#define BSP_CycleVid 82
#define BSP_IncFnt 83
#define BSP_DecFnt 84
#define BSP_IncBri 85
#define BSP_DecBri 86
#define BSP_MovSubUp 87
#define BSP_MovSubDown 88
#define BSP_SHTime 89
#define BSP_IncBriHW 90
#define BSP_DecBriHW 91
#define BSP_IncConHW 92
#define BSP_DecConHW 93
#define BSP_IncHueHW 94
#define BSP_DecHueHW 95
#define BSP_IncSatHW 96
#define BSP_DecSatHW 97
#define BSP_ShowHWClr 98
#define BSP_IncMovWin 99
#define BSP_DecMovWin 100
#define BSP_IncPBRate1 101
#define BSP_DecPBRate1 102
#define BSP_SWRepeat 103
#define BSP_SWDispFmt 104
#define BSP_FastForw 105
#define BSP_FastRew 106
#define BSP_OpenURL 107

#endif