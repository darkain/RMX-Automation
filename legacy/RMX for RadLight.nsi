!define VER_MAJOR 1
!define VER_MINOR 0
!define VER_REVISION "(20050215 BETA)"

!define name "RM-X Media Edition for RadLight"			;name of installation
!define plugins "Modules"					;output folder
!define rmxfile "rl_rmx.dll"					;output rmx core filename
!define rmxdir "RadLight"					;input folder
!define rmxinstall "RMX-RadLight Setup"				;output setup exe name


;--------------------------------
;Configuration

InstallDir "$PROGRAMFILES\RadLight LLC\RadLight 4 BETA 1\"
InstallDirRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\RadLight 4 BETA 1" "UninstallString"


;--------------------------------
;installer options and layout

!include "options.nsh"

;--------------------------------
;installer Section

!include "files.nsh"


;--------------------------------
;Uninstaller Section

!include "uninstall.nsh"
