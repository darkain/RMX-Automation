!include "..\rmx_version.h"

!define name "RMX Automation - Media Edition"			;name of installation
!define rmxdir "files"						;input folder
!define rmxinstall "rmx_media_edition-${VER_BUILD}"		;output setup exe name

!define MEDIAEDITION


;--------------------------------
;Configuration

Var pluginDir
Var rmxFile
Var instreg
Var instpath


;InstallDir "$instDir"
;InstallDirRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Winamp" "UninstallString"



;--------------------------------
;Version Information

  VIProductVersion "${VER_MAJOR}.${VER_MINOR}.0.0"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "RMX Automation - Visit http://rmx.sourceforge.net/ for more details"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Vincent E. Milum Jr."
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "RMX Automation"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "v${VER_MAJOR}.${VER_MINOR} ${VER_REVISION}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "InternalName" "RMX Automation"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "Copyright © 2003-2007 Vincent E. Milum Jr."
  VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "RMX Automation"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductVersion" "RMX Automation v${VER_MAJOR}.${VER_MINOR} ${VER_REVISION}"
  VIAddVersionKey /LANG=${LANG_ENGLISH} "SpecialBuild" "Media Edition"


;--------------------------------
;installer options and layout

!include "options.nsh"


;--------------------------------
;MSVC runtime environment

!include "runtime.nsh"


;--------------------------------
;installer Section

!include "files.nsh"


;--------------------------------
;Uninstaller Section

!include "uninstall.nsh"
