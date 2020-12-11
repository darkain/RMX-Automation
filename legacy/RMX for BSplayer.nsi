!define VER_MAJOR 1
!define VER_MINOR 0
!define VER_REVISION "(20050215 BETA)"

!define name "RM-X Media Edition for BSplayer"			;name of installation
!define plugins "plugins"					;output folder
!define rmxfile "bs_rmx.dll"					;output rmx core filename
!define rmxdir "BSplayer"					;input folder
!define rmxinstall "RMX-BSplayer Setup"				;output setup exe name


;--------------------------------
;Configuration

InstallDir "$PROGRAMFILES\Webteh\BSPlayer\"
InstallDirRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BSPlayer1" "UninstallString"


;--------------------------------
;installer options and layout

!include "options.nsh"


;--------------------------------
;Installer Sections

!include "files.nsh"


;--------------------------------
;Uninstaller Section

!include "uninstall.nsh"
