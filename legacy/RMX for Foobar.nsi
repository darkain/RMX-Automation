!define VER_MAJOR 1
!define VER_MINOR 0
!define VER_REVISION "(20050215 BETA)"

!define name "RM-X Media Edition for Foobar"			;name of installation
!define plugins "components"					;output folder
!define rmxfile "foo_rmx.dll"					;output rmx core filename
!define rmxdir "Foobar"						;input folder
!define rmxinstall "RMX-Foobar Setup"				;output setup exe name


;--------------------------------
;Configuration

InstallDir "$PROGRAMFILES\foobar2000\"
InstallDirRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\foobar2000" "UninstallString"


;--------------------------------
;installer options and layout

!include "options.nsh"


;--------------------------------
;installer Section

!include "files.nsh"


;--------------------------------
;Uninstaller Section

!include "uninstall.nsh"
