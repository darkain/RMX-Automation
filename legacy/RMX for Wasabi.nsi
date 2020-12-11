;todo - add start menu items

!define VER_MAJOR 1
!define VER_MINOR 0
!define VER_REVISION "Beta 7g"

;--------------------------------
;Include Modern UI
  !include "MUI.nsh"

;--------------------------------
;Configuration

  ;General
  !define name "RM-X Media Edition for wasabi.player"
  Name "${name}"
  OutFile "RMX-Wasabi Setup.exe"

  ;Folder selection page
  InstallDir "$PROGRAMFILES\Wasabi\"
  
  ;Get install folder from registry if available
  InstallDirRegKey HKLM \
                 "Software\Microsoft\Windows\CurrentVersion\Uninstall\Winamp3" \
                 "UninstallString"

  InstType "RM-X Core Only"
  InstType "RM-X Core and Plugins"
;  InstType "RM-X, Plugins, and Source Code"
;  InstType "NetSlave Client !ONLY!"
;  InstType "RM-X Lite for Winamp v2.x/5.x"
;  InstType "RM-X Lite for Winamp3 v1.x (build 488 thru 499)"


;--------------------------------
;Modern UI Configuration

  !define MUI_WELCOMEPAGE_TEXT "This wizard will guide you through the installation of RM-X Media Edition version ${VER_MAJOR}.${VER_MINOR} ${VER_REVISION}.\r\n\r\n\r\n$_CLICK."
  !define MUI_HEADERBITMAP "old stuff\header.bmp"
  !define MUI_SPECIALBITMAP "${NSISDIR}\Contrib\Graphics\Wizard\win.bmp"
  !define MUI_ABORTWARNING
  !define MUI_ICON "Old Stuff\rm-x.ico"
  !define MUI_UNICON "Old Stuff\uninstall.ico"
  !define MUI_COMPONENTSPAGE_SMALLDESC


;--------------------------------
;Pages
  ;!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\Docs\index.html"
  !define MUI_FINISHPAGE_LINK "Visit RM-X on the web for all of the latest news and builds."
  !define MUI_FINISHPAGE_LINK_LOCATION "http://darkain.skin-zone.net/"

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "ReadMe.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES



;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"


;--------------------------------
;Installer Sections

Section "RM-X Core" SecCore

  SetDetailsPrint textonly
  DetailPrint "Installing ${name}..."
  SetDetailsPrint listonly

  SectionIn 1 2 3 RO
  SetOverwrite on

  ;main RM-X EXE file
  SetOutPath "$INSTDIR\WACs"
  File "Wasabi\RM-X.wac"
  File "ReadMe.txt"

  ;create folders
  CreateDirectory "$INSTDIR\WACs\RMX Settings"
  CreateDirectory "$INSTDIR\WACs\RMX Plugins"
  
  ;Store install folder
  WriteRegStr HKCU "Software\RM-X Media Edition for Wasabi" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\RMX-Uninstall.exe"

SectionEnd

SubSection "Optional Plugins"

;  Section "RM-X Forward" SecForward
;    SectionIn 1 2 3
;    SetOverwrite on
;    SetOutPath "$INSTDIR\RMX Plugins"
;    File "SA\RMX Forward.dll"
;  SectionEnd
  
  Section "Test Plugin" SecTestPlugin
    SectionIn 1 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\WACs\RMX Plugins"
    File "plugins\test.dll"
  SectionEnd

  Section "Media Controller" SecMedControl
    SectionIn 1 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\WACs\RMX Plugins"
    File "plugins\Media Controller.dll"
  SectionEnd
  
  Section "IntelliRemote" SecIntelliRemote
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\WACs\RMX Plugins"
    File "plugins\IntelliRemote.dll"
  SectionEnd
  
  Section "Remote Center" SecRemoteCenter
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\WACs\RMX Plugins"
    File "plugins\RemoteCenter.dll"
  SectionEnd
  
  Section "Remote Wonder" SecRemoteWonder
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\WACs\RMX Plugins"
    File "plugins\ATI RemoteWonder.dll"
  SectionEnd

  Section "GBA System Stats" SecGba
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\WACs\RMX Plugins"
    File "plugins\GBA.dll"
  SectionEnd

  Section "Fraps Add-on" SecFraps
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\WACs\RMX Plugins"
    File "plugins\RmxFraps.dll"
  SectionEnd

  Section "PC-DVD Remote" SecPcDvd
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\WACs\RMX Plugins"
    File "plugins\PC-DVD.dll"
  SectionEnd
  
;  Section "HTTP Web Server" SecWebServer
;    SectionIn 2 3
;    SetOverwrite on
;    SetOutPath "$INSTDIR\RMX Plugins"
;    File "SA\WebServer.dll"
;    CreateDirectory "$INSTDIR\Web Site"
;  SectionEnd
  
;  Section "NetSlave Server" SecSlaveServer
;    SectionIn 2 3
;    SetOverwrite on
;    SetOutPath "$INSTDIR\RMX Plugins"
;    File "SA\NetSlave Server.dll"
;  SectionEnd
  
;  Section "NetSlave Client"
;    SectionIn 2 3
;    SetOverwrite on
;    SetOutPath "$INSTDIR\RMX Plugins"
;;   File "SA\NetSlave Server.dll"
;  SectionEnd

SubSectionEnd

!ifdef vbase_source
SubSection "vBase SDK (VB/C++/ASM Source Code)"

  Section "vBase Core Files (C++)"
    SectionIn 3
  SectionEnd

  Section "SendMessage (VB)"
    SectionIn 3
  SectionEnd

  Section "Media Controller (C++)"
    SectionIn 3
  SectionEnd

  SubSection "NetSlave (VB/C++)"
    Section "Stand-Alone Client (VB)"
      SectionIn 3
    SectionEnd

    Section "Stand-Alone Client (C++)"
      SectionIn 3
    SectionEnd

    Section "NetSlave Client RM-X Plugin (C++)"
      SectionIn 3
    SectionEnd

    Section "NetSlave Server RM-X Plugin (C++)"
      SectionIn 3
    SectionEnd
  SubSectionEnd 

  Section "vPlaylist2 (C++)"
    SectionIn 3
  SectionEnd

  Section "vPlyalist3 (C++)"
    SectionIn 3
  SectionEnd

  Section "XML (C++)"
    SectionIn 3
  SectionEnd

  Section "Play (ASM)"
    SectionIn 3
    SetOutPath "$INSTDIR\SDK"
    File "play.asm"
  SectionEnd

SubSectionEnd
!endif

;Section "NetSlave Stand-Alone Client" SecSlaveClient
;  SectionIn 4
;  SetOverwrite on
;  SetOutPath "$INSTDIR"
;  File "SA\NetSlave2.exe"
;SectionEnd



;--------------------------------
;Descriptions

  LangString DESC_SecCore ${LANG_ENGLISH} "RM-X Core Files - Required for all plugins"
  LangString DESC_SecMedControl ${LANG_ENGLISH} "Simple test plugin, used mainly for debuging purposes"
  LangString DESC_SecForward ${LANG_ENGLISH} "Required to communicate with RM-X Lite and Legacy versions of RM-X"
  LangString DESC_SecIntelliRemote ${LANG_ENGLISH} "Required for RM-900 support"
  LangString DESC_SecRemoteCenter ${LANG_ENGLISH} "Required to interact with Creative Remote Center - RM-1000, iR2000, iNFRA"
  LangString DESC_SecRemoteWonder ${LANG_ENGLISH} "Required to work with the ATI RemoteWonder - DLL must be loaded by both RM-X and ATI software"
  LangString DESC_SecWebServer ${LANG_ENGLISH} "HTTP Web Server for browsing and manipulating playlists"
  LangString DESC_SecSlaveServer ${LANG_ENGLISH} "NetSlave Server plugin for RM-X"
  LangString DESC_SecSlaveClient ${LANG_ENGLISH} "NetSlave Client stand-alone application"
  LangString DESC_SecPcDvd ${LANG_ENGLISH} "Creative PC-DVD iR Remote"
  LangString DESC_SecGba ${LANG_ENGLISH} "Displays various information on a GBA screen via MBV2 Cable"
  LangString DESC_SecFraps ${LANG_ENGLISH} "Used to record framerate information from Fraps"
  LangString DESC_SecTestPlugin ${LANG_ENGLISH} "Test plugin to help debug RMX features"

  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecCore} $(DESC_SecCore)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecForward} $(DESC_SecForward)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMedControl} $(DESC_SecMedControl)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecIntelliRemote} $(DESC_SecIntelliRemote)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecRemoteCenter} $(DESC_SecRemoteCenter)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecRemoteWonder} $(DESC_SecRemoteWonder)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecWebServer} $(DESC_SecWebServer)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecSlaveServer} $(DESC_SecSlaveServer)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecSlaveClient} $(DESC_SecSlaveClient)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecPcDvd} $(DESC_SecPcDvd)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecGba} $(DESC_SecGba)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecFraps} $(DESC_SecFraps)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecTestPlugin $(DESC_SecTestPlugin)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END


;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\Uninstall.exe"

  Delete "$INSTDIR\WACs\RMX Plugins\RmxFraps.dll"
  Delete "$INSTDIR\WACs\RMX Plugins\GBA.dll"
  Delete "$INSTDIR\WACs\RMX Plugins\Media Controller.dll"
  Delete "$INSTDIR\WACs\RMX Plugins\RMX Forward.dll"
  Delete "$INSTDIR\WACs\RMX Plugins\IntelliRemote.dll"
  Delete "$INSTDIR\WACs\RMX Plugins\RemoteCenter.dll"
  Delete "$INSTDIR\WACs\RMX Plugins\ATI RemoteWonder.dll"
  Delete "$INSTDIR\WACs\RMX Plugins\WebServer.dll"
  Delete "$INSTDIR\WACs\RMX Plugins\NetSlave Server.dll"
  Delete "$INSTDIR\WACs\RMX Plugins\PC-DVD.dll"
  Delete "$INSTDIR\WACs\NetSlave2.exe"
  Delete "$INSTDIR\WACs\RMX SA.exe"
  Delete "$INSTDIR\WACs\readme.txt"

  RMDir "$INSTDIR\WACs\RMX Settings"
  RMDir "$INSTDIR\WACs\RMX Plugins"
;  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\RM-X Media Edition for Wasabi"

SectionEnd
