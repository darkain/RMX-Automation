# Microsoft Developer Studio Project File - Name="RMX SDK" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=RMX SDK - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rmx_sdk.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rmx_sdk.mak" CFG="RMX SDK - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RMX SDK - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "RMX SDK - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RMX SDK - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe
F90=df.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "RMX SDK - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe
F90=df.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "RMX SDK - Win32 Release"
# Name "RMX SDK - Win32 Debug"
# Begin Group "cb"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cb\cbBase.cpp
# End Source File
# Begin Source File

SOURCE=.\cb\cbBase.h
# End Source File
# Begin Source File

SOURCE=.\cb\cbCore.cpp
# End Source File
# Begin Source File

SOURCE=.\cb\cbCore.h
# End Source File
# Begin Source File

SOURCE=.\cb\cbCoreDelay.h
# End Source File
# Begin Source File

SOURCE=.\cb\cbList.cpp
# End Source File
# Begin Source File

SOURCE=.\cb\cbList.h
# End Source File
# Begin Source File

SOURCE=.\cb\cbListAuto.h
# End Source File
# Begin Source File

SOURCE=.\cb\cbListItem.cpp
# End Source File
# Begin Source File

SOURCE=.\cb\cbListItem.h
# End Source File
# Begin Source File

SOURCE=.\cb\cbListRoot.cpp
# End Source File
# Begin Source File

SOURCE=.\cb\cbListRoot.h
# End Source File
# Begin Source File

SOURCE=.\cb\cbPlugin.h
# End Source File
# Begin Source File

SOURCE=.\cb\cbRoot.h
# End Source File
# End Group
# Begin Group "cfg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cfg\cfgAddChild.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgAutoList.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgBase.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgBase.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgBool.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgBool.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgButton.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgButton.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgButtonMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgButtonMenu.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgHelper.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgInt.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgInt.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgLine.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgLink.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgLink.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgMenu.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgMessage.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgNull.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgNull.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgPath.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgPath.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgRoot.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgRoot.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgText.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgText.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgTree.cpp
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgTree.h
# End Source File
# Begin Source File

SOURCE=.\cfg\cfgWndHelper.h
# End Source File
# Begin Source File

SOURCE=.\cfg\config.h
# End Source File
# End Group
# Begin Group "cons"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\cons\consBase.cpp
# End Source File
# Begin Source File

SOURCE=.\cons\consBase.h
# End Source File
# End Group
# Begin Group "dx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\dx\dxRender.h
# End Source File
# Begin Source File

SOURCE=.\dx\dxSprite.cpp
# End Source File
# Begin Source File

SOURCE=.\dx\dxSprite.h
# End Source File
# Begin Source File

SOURCE=.\dx\dxTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\dx\dxTexture.h
# End Source File
# Begin Source File

SOURCE=.\dx\dxTilemap.cpp
# End Source File
# Begin Source File

SOURCE=.\dx\dxTilemap.h
# End Source File
# Begin Source File

SOURCE=.\dx\dxTileset.cpp
# End Source File
# Begin Source File

SOURCE=.\dx\dxTileset.h
# End Source File
# Begin Source File

SOURCE=.\dx\dxWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\dx\dxWnd.h
# End Source File
# Begin Source File

SOURCE=.\dx\vDirectX.h
# End Source File
# End Group
# Begin Group "evnt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\evnt\evntAbsolute.cpp
# End Source File
# Begin Source File

SOURCE=.\evnt\evntAbsolute.h
# End Source File
# Begin Source File

SOURCE=.\evnt\evntBase.cpp
# End Source File
# Begin Source File

SOURCE=.\evnt\evntBase.h
# End Source File
# Begin Source File

SOURCE=.\evnt\evntLock.cpp
# End Source File
# Begin Source File

SOURCE=.\evnt\evntLock.h
# End Source File
# Begin Source File

SOURCE=.\evnt\evntPress.cpp
# End Source File
# Begin Source File

SOURCE=.\evnt\evntPress.h
# End Source File
# Begin Source File

SOURCE=.\evnt\evntRelative.cpp
# End Source File
# Begin Source File

SOURCE=.\evnt\evntRelative.h
# End Source File
# Begin Source File

SOURCE=.\evnt\evntSingle.cpp
# End Source File
# Begin Source File

SOURCE=.\evnt\evntSingle.h
# End Source File
# End Group
# Begin Group "func"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\func\funcAbsolute.cpp
# End Source File
# Begin Source File

SOURCE=.\func\funcAbsolute.h
# End Source File
# Begin Source File

SOURCE=.\func\funcBase.cpp
# End Source File
# Begin Source File

SOURCE=.\func\funcBase.h
# End Source File
# Begin Source File

SOURCE=.\func\funcCallback.cpp
# End Source File
# Begin Source File

SOURCE=.\func\funcCallback.h
# End Source File
# Begin Source File

SOURCE=.\func\funcGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\func\funcGroup.h
# End Source File
# Begin Source File

SOURCE=.\func\funcParam.cpp
# End Source File
# Begin Source File

SOURCE=.\func\funcParam.h
# End Source File
# Begin Source File

SOURCE=.\func\funcPress.cpp
# End Source File
# Begin Source File

SOURCE=.\func\funcPress.h
# End Source File
# Begin Source File

SOURCE=.\func\funcRelative.cpp
# End Source File
# Begin Source File

SOURCE=.\func\funcRelative.h
# End Source File
# Begin Source File

SOURCE=.\func\funcSingle.cpp
# End Source File
# Begin Source File

SOURCE=.\func\funcSingle.h
# End Source File
# Begin Source File

SOURCE=.\func\funcToggle.cpp
# End Source File
# Begin Source File

SOURCE=.\func\funcToggle.h
# End Source File
# End Group
# Begin Group "handles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\handles\hconsole.h
# End Source File
# Begin Source File

SOURCE=.\handles\hfunction.h
# End Source File
# Begin Source File

SOURCE=.\handles\hparamlist.h
# End Source File
# Begin Source File

SOURCE=.\handles\hplugin.h
# End Source File
# End Group
# Begin Group "prm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\prm\prmBase.cpp
# End Source File
# Begin Source File

SOURCE=.\prm\prmBase.h
# End Source File
# End Group
# Begin Group "src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ColorSpace.h
# End Source File
# Begin Source File

SOURCE=.\guiddef.h
# End Source File
# Begin Source File

SOURCE=.\plugBase.h
# End Source File
# Begin Source File

SOURCE=.\rmxBase.cpp
# End Source File
# Begin Source File

SOURCE=.\rmxBase.h
# End Source File
# Begin Source File

SOURCE=.\rmxData.cpp
# End Source File
# Begin Source File

SOURCE=.\rmxData.h
# End Source File
# Begin Source File

SOURCE=.\rmxGuid.cpp
# End Source File
# Begin Source File

SOURCE=.\rmxGuid.h
# End Source File
# Begin Source File

SOURCE=.\rmxList.h
# End Source File
# Begin Source File

SOURCE=.\rmxLock.h
# End Source File
# Begin Source File

SOURCE=.\rmxMsg.h
# End Source File
# Begin Source File

SOURCE=.\rmxMutex.cpp
# End Source File
# Begin Source File

SOURCE=.\rmxMutex.h
# End Source File
# Begin Source File

SOURCE=.\rmxName.h
# End Source File
# Begin Source File

SOURCE=.\rmxOS.h
# End Source File
# Begin Source File

SOURCE=.\rmxPair.h
# End Source File
# Begin Source File

SOURCE=.\rmxString.cpp
# End Source File
# Begin Source File

SOURCE=.\rmxString.h
# End Source File
# Begin Source File

SOURCE=.\rmxThread.cpp
# End Source File
# Begin Source File

SOURCE=.\rmxThread.h
# End Source File
# Begin Source File

SOURCE=.\rmxTypes.h
# End Source File
# Begin Source File

SOURCE=.\rmxUnicode.cpp
# End Source File
# Begin Source File

SOURCE=.\rmxUnicode.h
# End Source File
# Begin Source File

SOURCE=.\StringList.h
# End Source File
# Begin Source File

SOURCE=.\TextAlign.h
# End Source File
# End Group
# Begin Group "whk"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\whk\whkBase.cpp
# End Source File
# Begin Source File

SOURCE=.\whk\whkBase.h
# End Source File
# Begin Source File

SOURCE=.\whk\whkColor.cpp
# End Source File
# Begin Source File

SOURCE=.\whk\whkColor.h
# End Source File
# Begin Source File

SOURCE=.\whk\whkCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\whk\whkCommand.h
# End Source File
# Begin Source File

SOURCE=.\whk\whkMouse.cpp
# End Source File
# Begin Source File

SOURCE=.\whk\whkMouse.h
# End Source File
# Begin Source File

SOURCE=.\whk\whkNotify.cpp
# End Source File
# Begin Source File

SOURCE=.\whk\whkNotify.h
# End Source File
# Begin Source File

SOURCE=.\whk\whkScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\whk\whkScroll.h
# End Source File
# Begin Source File

SOURCE=.\whk\whkTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\whk\whkTrack.h
# End Source File
# End Group
# Begin Group "win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\win32\Comm.cpp
# End Source File
# Begin Source File

SOURCE=.\Win32\Comm.h
# End Source File
# Begin Source File

SOURCE=.\win32\manifest.h
# End Source File
# Begin Source File

SOURCE=.\win32\Menu.cpp
# End Source File
# Begin Source File

SOURCE=.\Win32\Menu.h
# End Source File
# Begin Source File

SOURCE=.\win32\MessageWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Win32\MessageWnd.h
# End Source File
# Begin Source File

SOURCE=.\win32\Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\Win32\Registry.h
# End Source File
# Begin Source File

SOURCE=.\win32\Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\Win32\Thread.h
# End Source File
# Begin Source File

SOURCE=.\win32\Timer.cpp
# End Source File
# Begin Source File

SOURCE=.\Win32\Timer.h
# End Source File
# Begin Source File

SOURCE=.\win32\timerDelete.h
# End Source File
# Begin Source File

SOURCE=.\win32\TrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\Win32\TrayIcon.h
# End Source File
# Begin Source File

SOURCE=.\win32\Win32.cpp
# End Source File
# Begin Source File

SOURCE=.\Win32\Win32.h
# End Source File
# End Group
# Begin Group "wnd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\wnd\wndBase.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndBase.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndDialog.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndDivider.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndDivider.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndDroplist.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndDroplist.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndEdit.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndForest.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndForest.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndForestBase.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndForestBase.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndForestItem.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndForestItem.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndForestItems.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndForestItems.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndFrame.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndLabel.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndLabel.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndList.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndList.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndScroll.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndScroll.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndStatus.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndSubclass.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndSubclass.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndTab.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndTab.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndTree.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndTree.h
# End Source File
# Begin Source File

SOURCE=.\wnd\wndTreeItem.cpp
# End Source File
# Begin Source File

SOURCE=.\wnd\wndTreeItem.h
# End Source File
# End Group
# Begin Group "xml"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\xml\amigaconfig.h
# End Source File
# Begin Source File

SOURCE=.\xml\ascii.h
# End Source File
# Begin Source File

SOURCE=.\xml\asciitab.h
# End Source File
# Begin Source File

SOURCE=.\xml\expat.h
# End Source File
# Begin Source File

SOURCE=.\xml\expat_external.h
# End Source File
# Begin Source File

SOURCE=.\xml\iasciitab.h
# End Source File
# Begin Source File

SOURCE=.\xml\internal.h
# End Source File
# Begin Source File

SOURCE=.\xml\latin1tab.h
# End Source File
# Begin Source File

SOURCE=.\xml\macconfig.h
# End Source File
# Begin Source File

SOURCE=.\xml\nametab.h
# End Source File
# Begin Source File

SOURCE=.\xml\utf8tab.h
# End Source File
# Begin Source File

SOURCE=.\xml\winconfig.h
# End Source File
# Begin Source File

SOURCE=.\xml\xmlparse.c
# End Source File
# Begin Source File

SOURCE=.\xml\xmlread.cpp
# End Source File
# Begin Source File

SOURCE=.\xml\xmlread.h
# End Source File
# Begin Source File

SOURCE=.\xml\xmlreadfile.cpp
# End Source File
# Begin Source File

SOURCE=.\xml\xmlreadfile.h
# End Source File
# Begin Source File

SOURCE=.\xml\xmlrole.c
# End Source File
# Begin Source File

SOURCE=.\xml\xmlrole.h
# End Source File
# Begin Source File

SOURCE=.\xml\xmltok.c
# End Source File
# Begin Source File

SOURCE=.\xml\xmltok.h
# End Source File
# Begin Source File

SOURCE=.\xml\xmlwrite.cpp
# End Source File
# Begin Source File

SOURCE=.\xml\xmlwrite.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\license.txt
# End Source File
# Begin Source File

SOURCE=..\predefine.h
# End Source File
# Begin Source File

SOURCE=..\rmx_readme.txt
# End Source File
# Begin Source File

SOURCE=..\rmx_version.h
# End Source File
# End Target
# End Project
