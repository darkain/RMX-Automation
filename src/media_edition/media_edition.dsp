# Microsoft Developer Studio Project File - Name="RMX Media Edition" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=RMX Media Edition - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "media_edition.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "media_edition.mak" CFG="RMX Media Edition - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RMX Media Edition - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "RMX Media Edition - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RMX Media Edition - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RMX Media Edition_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RMX Media Edition_EXPORTS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"\Program Files\Winamp\Plugins\gen_RMX.dll"

!ELSEIF  "$(CFG)" == "RMX Media Edition - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RMX Media Edition_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RMX Media Edition_EXPORTS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"\Program Files\Winamp\Plugins\gen_RMX.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "RMX Media Edition - Win32 Release"
# Name "RMX Media Edition - Win32 Debug"
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\common.cpp
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\playlistFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\playlistFunction.h
# End Source File
# Begin Source File

SOURCE=.\pluginPlaylist.cpp
# End Source File
# Begin Source File

SOURCE=.\pluginPlaylist.h
# End Source File
# End Group
# Begin Group "bsplayer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bsplayer\bsp.h
# End Source File
# Begin Source File

SOURCE=.\bsplayer\bspMain.cpp
# End Source File
# Begin Source File

SOURCE=.\bsplayer\bspMain.h
# End Source File
# Begin Source File

SOURCE=.\bsplayer\bspPlaylistPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\bsplayer\bspPlaylistPlugin.h
# End Source File
# Begin Source File

SOURCE=.\bsplayer\bspplg.h
# End Source File
# End Group
# Begin Group "foobar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\foobar2000\fbMain.cpp
# End Source File
# Begin Source File

SOURCE=.\foobar2000\fbMain.h
# End Source File
# Begin Source File

SOURCE=.\foobar2000\fbPlaylistPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\foobar2000\fbPlaylistPlugin.h
# End Source File
# End Group
# Begin Group "radlight"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\radlight\RadLight.h
# End Source File
# Begin Source File

SOURCE=.\radlight\rlMain.cpp
# End Source File
# Begin Source File

SOURCE=.\radlight\rlMain.h
# End Source File
# Begin Source File

SOURCE=.\radlight\rlPlaylistPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\radlight\rlPlaylistPlugin.h
# End Source File
# Begin Source File

SOURCE=.\radlight\URLBase.cpp
# End Source File
# Begin Source File

SOURCE=.\radlight\URLBase.h
# End Source File
# Begin Source File

SOURCE=.\radlight\URLPlayback.h
# End Source File
# Begin Source File

SOURCE=.\radlight\URLTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\radlight\URLTypes.h
# End Source File
# End Group
# Begin Group "winamp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\winamp\gen.h
# End Source File
# Begin Source File

SOURCE=.\winamp\ipc_pe.h
# End Source File
# Begin Source File

SOURCE=.\winamp\wa_dlg.h
# End Source File
# Begin Source File

SOURCE=.\winamp\wa_hotkeys.h
# End Source File
# Begin Source File

SOURCE=.\winamp\wa_ipc.h
# End Source File
# Begin Source File

SOURCE=.\winamp\waMain.cpp
# End Source File
# Begin Source File

SOURCE=.\winamp\waMain.h
# End Source File
# Begin Source File

SOURCE=.\winamp\waPlaylistPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\winamp\waPlaylistPlugin.h
# End Source File
# End Group
# Begin Group "wmp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\wmp\wmp.h
# End Source File
# Begin Source File

SOURCE=.\wmp\wmpMain.cpp
# End Source File
# Begin Source File

SOURCE=.\wmp\wmpMain.h
# End Source File
# Begin Source File

SOURCE=.\wmp\wmpPlaylistPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\wmp\wmpPlaylistPlugin.h
# End Source File
# Begin Source File

SOURCE=.\wmp\wmpplug.h
# End Source File
# Begin Source File

SOURCE=.\wmp\wmptest.rgs
# End Source File
# End Group
# Begin Group "resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\experts.def
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# End Group
# Begin Source File

SOURCE="..\..\Installer\Icons\rm-x.ico"
# End Source File
# Begin Source File

SOURCE=..\..\Installer\Icons\uninstall.ico
# End Source File
# Begin Source File

SOURCE=.\wmptest.rgs
# End Source File
# End Target
# End Project
