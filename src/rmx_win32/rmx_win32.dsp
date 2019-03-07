# Microsoft Developer Studio Project File - Name="rmx_win32" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=rmx_win32 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rmx_win32.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rmx_win32.mak" CFG="rmx_win32 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rmx_win32 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "rmx_win32 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rmx_win32 - Win32 Release"

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

!ELSEIF  "$(CFG)" == "rmx_win32 - Win32 Debug"

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

# Name "rmx_win32 - Win32 Release"
# Name "rmx_win32 - Win32 Debug"
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\rmx_win32.cpp
# End Source File
# Begin Source File

SOURCE=.\rmx_win32.h
# End Source File
# End Group
# Begin Group "vConsoleWnd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\vConsoleWnd\ConsoleWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\vConsoleWnd\ConsoleWnd.h
# End Source File
# End Group
# Begin Group "vConfigWnd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\vConfigWnd\ConfigPaneItem.cpp
# End Source File
# Begin Source File

SOURCE=.\vConfigWnd\ConfigPaneItem.h
# End Source File
# Begin Source File

SOURCE=.\vConfigWnd\ConfigPaneItems.cpp
# End Source File
# Begin Source File

SOURCE=.\vConfigWnd\ConfigPaneItems.h
# End Source File
# Begin Source File

SOURCE=.\vConfigWnd\ConfigPaneWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\vConfigWnd\ConfigPaneWnd.h
# End Source File
# Begin Source File

SOURCE=.\vConfigWnd\ConfigStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\vConfigWnd\ConfigStatus.h
# End Source File
# Begin Source File

SOURCE=.\vConfigWnd\ConfigWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\vConfigWnd\ConfigWnd.h
# End Source File
# Begin Source File

SOURCE=.\vConfigWnd\ConfigWndTree.cpp
# End Source File
# Begin Source File

SOURCE=.\vConfigWnd\ConfigWndTree.h
# End Source File
# Begin Source File

SOURCE=.\vConfigWnd\TreeRootCallback.h
# End Source File
# End Group
# Begin Group "vSettingWnd"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\vSettingWnd\SettingWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\vSettingWnd\SettingWnd.h
# End Source File
# End Group
# Begin Group "vMasterPlugin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\vMasterPlugin\MasterConsole.h
# End Source File
# Begin Source File

SOURCE=.\vMasterPlugin\MasterEnable.h
# End Source File
# Begin Source File

SOURCE=.\vMasterPlugin\MasterExit.h
# End Source File
# Begin Source File

SOURCE=.\vMasterPlugin\MasterMenu.h
# End Source File
# Begin Source File

SOURCE=.\vMasterPlugin\MasterPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\vMasterPlugin\MasterPlugin.h
# End Source File
# Begin Source File

SOURCE=.\vMasterPlugin\MasterTray.cpp
# End Source File
# Begin Source File

SOURCE=.\vMasterPlugin\MasterTray.h
# End Source File
# Begin Source File

SOURCE=.\vMasterPlugin\MasterWnd.h
# End Source File
# End Group
# End Target
# End Project
