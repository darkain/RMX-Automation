# Microsoft Developer Studio Project File - Name="jnetlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=jnetlib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "jnetlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "jnetlib.mak" CFG="jnetlib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "jnetlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "jnetlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "jnetlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "jnetlib___Win32_Release"
# PROP BASE Intermediate_Dir "jnetlib___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "jnetlib___Win32_Release"
# PROP Intermediate_Dir "jnetlib___Win32_Release"
# PROP Target_Dir ""
F90=df.exe
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "jnetlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "jnetlib___Win32_Debug"
# PROP BASE Intermediate_Dir "jnetlib___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "jnetlib___Win32_Debug"
# PROP Intermediate_Dir "jnetlib___Win32_Debug"
# PROP Target_Dir ""
F90=df.exe
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
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

# Name "jnetlib - Win32 Release"
# Name "jnetlib - Win32 Debug"
# Begin Source File

SOURCE=.\asyncdns.cpp
# End Source File
# Begin Source File

SOURCE=.\asyncdns.h
# End Source File
# Begin Source File

SOURCE=.\connection.cpp
# End Source File
# Begin Source File

SOURCE=.\connection.h
# End Source File
# Begin Source File

SOURCE=.\httpget.cpp
# End Source File
# Begin Source File

SOURCE=.\httpget.h
# End Source File
# Begin Source File

SOURCE=.\httppost.cpp
# End Source File
# Begin Source File

SOURCE=.\httppost.h
# End Source File
# Begin Source File

SOURCE=.\httpserv.cpp
# End Source File
# Begin Source File

SOURCE=.\httpserv.h
# End Source File
# Begin Source File

SOURCE=.\ircclient.cpp
# End Source File
# Begin Source File

SOURCE=.\ircclient.h
# End Source File
# Begin Source File

SOURCE=.\jnetlib.h
# End Source File
# Begin Source File

SOURCE=.\listen.cpp
# End Source File
# Begin Source File

SOURCE=.\listen.h
# End Source File
# Begin Source File

SOURCE=.\netinc.h
# End Source File
# Begin Source File

SOURCE=.\sslconnection.cpp
# End Source File
# Begin Source File

SOURCE=.\sslconnection.h
# End Source File
# Begin Source File

SOURCE=.\ssllisten.cpp
# End Source File
# Begin Source File

SOURCE=.\ssllisten.h
# End Source File
# Begin Source File

SOURCE=.\udpconnection.h
# End Source File
# Begin Source File

SOURCE=.\util.cpp
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\webserver.cpp
# End Source File
# Begin Source File

SOURCE=.\webserver.h
# End Source File
# End Target
# End Project
