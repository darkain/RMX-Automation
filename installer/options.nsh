;General
Name "${name}"
OutFile "${rmxinstall}.exe"


SetCompressor /SOLID lzma


;--------------------------------
;Include Modern UI
  !include "MUI.nsh"


  InstType "RMX Automation Core Only"
;  InstType "RM-X Core and Plugins"



;--------------------------------
;Modern UI Configuration

  !define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of ${name} version ${VER_MAJOR}.${VER_MINOR} ${VER_REVISION}.\r\n\r\n\r\n$_CLICK."
  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP "header.bmp"
;  !define MUI_SPECIALBITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
  !define MUI_ABORTWARNING
  !define MUI_ICON "..\resources\icons\rmx.ico"
  !define MUI_UNICON "..\resources\icons\uninstall.ico"
  !define MUI_COMPONENTSPAGE_SMALLDESC


;--------------------------------
;Pages
  ;!define MUI_FINISHPAGE_SHOWREADME "http://rmx.darkain.com/wiki/RM-X"
  !define MUI_FINISHPAGE_LINK "Click here to visit the RMX Automation web site for updates"
  !define MUI_FINISHPAGE_LINK_LOCATION "http://rmx.sourceforge.net/"

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "..\rmx_readme.txt"
!ifdef MEDIAEDITION
  Page custom playerSelect ;Custom page
!endif
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES



!ifdef MEDIAEDITION

Var rmxplayer


Function .onInit
  !insertmacro MUI_INSTALLOPTIONS_EXTRACT "page.ini"
  File /oname=$PLUGINSDIR\winamp.ico    ..\resources\media_icons\winamp.ico
  File /oname=$PLUGINSDIR\wmp.ico       ..\resources\media_icons\wmp.ico
  File /oname=$PLUGINSDIR\foobar.ico    ..\resources\media_icons\foobar.ico
  File /oname=$PLUGINSDIR\radlight.ico  ..\resources\media_icons\radlight.ico
  File /oname=$PLUGINSDIR\bsplayer.ico  ..\resources\media_icons\bsplayer.ico
  !insertmacro MUI_INSTALLOPTIONS_WRITE "page.ini" "Field 1" "Text" "$PLUGINSDIR\winamp.ico"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "page.ini" "Field 2" "Text" "$PLUGINSDIR\wmp.ico"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "page.ini" "Field 3" "Text" "$PLUGINSDIR\foobar.ico"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "page.ini" "Field 4" "Text" "$PLUGINSDIR\radlight.ico"
  !insertmacro MUI_INSTALLOPTIONS_WRITE "page.ini" "Field 5" "Text" "$PLUGINSDIR\bsplayer.ico"
FunctionEnd




; Trim
;   Removes leading & trailing whitespace from a string
; Usage:
;   Push
;   Call Trim
;   Pop
Function Trim
	Exch $R1 ; Original string
	Push $R2
	Push $R3
 
Loop:
	StrCpy $R2 "$R1" 1
	StrCmp "$R2" '"' TrimLeft
	StrCmp "$R2" " " TrimLeft
	StrCmp "$R2" "$\r" TrimLeft
	StrCmp "$R2" "$\n" TrimLeft
	StrCmp "$R2" "	" TrimLeft ; this is a tab.
	GoTo Loop2
TrimLeft:
	StrCpy $R1 "$R1" "" 1
	Goto Loop
 
Loop2:
	StrCpy $R2 "$R1" 1 -1
	StrCmp "$R2" '"' TrimRight
	StrCmp "$R2" " " TrimRight
	StrCmp "$R2" "$\r" TrimRight
	StrCmp "$R2" "$\n" TrimRight
	StrCmp "$R2" "	" TrimRight ; this is a tab
	GoTo Loop3
TrimRight:
	StrCpy $R1 "$R1" -1
	Goto Loop2

Loop3:
	StrLen $R3 $R1
	IntCmp $R3 0 Done
	StrCpy $R2 "$R1" 1 -1
	StrCmp "$R2" "\" Done
	StrCpy $R1 "$R1" -1
	Goto Loop3
 
Done:
	Pop $R3
	Pop $R2
	Exch $R1
FunctionEnd




Function playerSelect 
  !insertmacro MUI_HEADER_TEXT "Player Selection" "Please choose the media player that you wish to install ${name} on."

  StrCpy $R0 "0"

loop:
  !insertmacro MUI_INSTALLOPTIONS_DISPLAY_RETURN "page.ini"
  Pop $R2
  StrCmp $R2 "success" "" done

  ReadINIStr $R1 "$PLUGINSDIR\page.ini" "Field 6" "State"
  StrCmp $R1 "1" winampproc

  ReadINIStr $R1 "$PLUGINSDIR\page.ini" "Field 7" "State"
  StrCmp $R1 "1" wmpproc

  ReadINIStr $R1 "$PLUGINSDIR\page.ini" "Field 8" "State"
  StrCmp $R1 "1" foobarproc

  ReadINIStr $R1 "$PLUGINSDIR\page.ini" "Field 9" "State"
  StrCmp $R1 "1" radlightproc

  ReadINIStr $R1 "$PLUGINSDIR\page.ini" "Field 10" "State"
  StrCmp $R1 "1" bsplayerproc


  MessageBox MB_ICONEXCLAMATION|MB_OK "Please select your media player for installation of ${name}"
  Goto loop


winampproc:
  ReadRegStr $instreg HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Winamp" "UninstallString"
  StrCpy $instpath "$PROGRAMFILES\Winamp"
  StrCpy $pluginDir "Plugins"
  StrCpy $rmxFile "gen_RMX.dll"
  StrCpy $rmxplayer "1"
  Goto done

wmpproc:
  StrCpy $instDir "$SYSDIR"
  StrCpy $pluginDir "RMX for WMP"
  StrCpy $rmxFile "wmp_RMX.dll"
  StrCpy $rmxplayer "2"
  Goto doneok

foobarproc:
  ReadRegStr $instreg HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\foobar2000" "UninstallString"
  StrCpy $instpath "$PROGRAMFILES\foobar2000"
  StrCpy $pluginDir "components"
  StrCpy $rmxFile "foo_RMX.dll"
  StrCpy $rmxplayer "3"
  Goto done

radlightproc:
  ReadRegStr $instreg HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RadLight 4.0" "UninstallString"
  StrCpy $instpath "$PROGRAMFILES\RadLight Company\RadLight 4.0"
  StrCpy $pluginDir "Modules"
  StrCpy $rmxFile "rl_RMX.dll"
  StrCpy $rmxplayer "4"
  Goto done

bsplayerproc:
  ReadRegStr $instreg HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\BSplayer1" "UninstallString"
  StrCpy $instpath "$PROGRAMFILES\Webteh\BSplayer"
  StrCpy $pluginDir "plugins"
  StrCpy $rmxFile "bs_RMX.dll"
  StrCpy $rmxplayer "5"
  Goto done


done:
  StrLen $R1 $instreg
  IntCmp $R1 0 doneerr
  push $instreg
  call Trim
  pop $instreg
  StrLen $R1 $instreg
  IntCmp $R1 0 doneerr
  StrCpy $instDir $instreg
  Goto doneok

doneerr:
  StrCpy $instDir $instpath
  Goto doneok

doneok:
FunctionEnd 


!endif


;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"
