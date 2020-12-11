# Microsoft Developer Studio Project File - Name="RMX Core" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=RMX Core - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rmx_core.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rmx_core.mak" CFG="RMX Core - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RMX Core - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "RMX Core - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RMX Core - Win32 Release"

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

!ELSEIF  "$(CFG)" == "RMX Core - Win32 Debug"

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

# Name "RMX Core - Win32 Release"
# Name "RMX Core - Win32 Debug"
# Begin Group "Callbacks"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\callbacks\cbConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\callbacks\cbConsole.h
# End Source File
# Begin Source File

SOURCE=.\callbacks\cbFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\callbacks\cbFunction.h
# End Source File
# Begin Source File

SOURCE=.\callbacks\cbPlugin.h
# End Source File
# Begin Source File

SOURCE=.\callbacks\plugCallback.h
# End Source File
# End Group
# Begin Group "IPC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ipc\ipcConfigCallback.h
# End Source File
# Begin Source File

SOURCE=.\ipc\ipcParser.cpp
# End Source File
# Begin Source File

SOURCE=.\ipc\ipcParser.h
# End Source File
# Begin Source File

SOURCE=.\ipc\ipcPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\ipc\ipcPlugin.h
# End Source File
# Begin Source File

SOURCE=.\ipc\ipcRootListCallback.h
# End Source File
# End Group
# Begin Group "Lua"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lua\luaConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\lua\luaConfig.h
# End Source File
# Begin Source File

SOURCE=.\lua\luaConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\lua\luaConsole.h
# End Source File
# Begin Source File

SOURCE=.\lua\luaFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\lua\luaFunction.h
# End Source File
# Begin Source File

SOURCE=.\lua\luaHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\lua\luaHelper.h
# End Source File
# Begin Source File

SOURCE=.\lua\luaPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\lua\luaPlugin.h
# End Source File
# Begin Source File

SOURCE=.\lua\luaTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\lua\luaTimer.h
# End Source File
# End Group
# Begin Group "Parser"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Parser.cpp
# End Source File
# Begin Source File

SOURCE=.\RmxStack.cpp
# End Source File
# Begin Source File

SOURCE=.\RmxStack.h
# End Source File
# End Group
# Begin Group "Plugin Loaders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LocalPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\LocalPlugin.h
# End Source File
# Begin Source File

SOURCE=.\NetLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\NetLoader.h
# End Source File
# Begin Source File

SOURCE=.\Plugin.cpp
# End Source File
# Begin Source File

SOURCE=.\Plugin.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\RmxPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\RmxPlugin.h
# End Source File
# Begin Source File

SOURCE=.\SecureLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\SecureLoader.h
# End Source File
# Begin Source File

SOURCE=.\SecureParser.cpp
# End Source File
# Begin Source File

SOURCE=.\SecureParser.h
# End Source File
# End Group
# Begin Group "XML"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\xml\xmlplugin.h
# End Source File
# Begin Source File

SOURCE=.\xml\xmlSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\xml\xmlSettings.h
# End Source File
# End Group
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\coreSettings.h
# End Source File
# Begin Source File

SOURCE=.\funcNone.h
# End Source File
# Begin Source File

SOURCE=.\Loader.cpp
# End Source File
# Begin Source File

SOURCE=.\Loader.h
# End Source File
# Begin Source File

SOURCE=.\PluginTree.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginTree.h
# End Source File
# End Group
# End Target
# End Project
