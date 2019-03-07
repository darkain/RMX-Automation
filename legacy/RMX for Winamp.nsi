!define VER_MAJOR 1
!define VER_MINOR 0
!define VER_REVISION "(20050215 BETA)"

!define name "RM-X Media Edition for Winamp"			;name of installation
!define plugins "plugins"					;output folder
!define rmxfile "gen_rmx.dll"					;output rmx core filename
!define rmxdir "Winamp"						;input folder
!define rmxinstall "RMX-Winamp Setup"				;output setup exe name


;--------------------------------
;Configuration

InstallDir "$PROGRAMFILES\Winamp\"
InstallDirRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Winamp" "UninstallString"


;--------------------------------
;installer options and layout

!include "options.nsh"

;--------------------------------
;installer Section

!include "files.nsh"


;--------------------------------
;Uninstaller Section

!include "uninstall.nsh"
