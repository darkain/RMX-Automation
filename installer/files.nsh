;--------------------------------
;Installer Sections

Section "RMX Automation" SecCore

  SetDetailsPrint textonly
  DetailPrint "Installing ${name}..."
  SetDetailsPrint listonly

  Call GetRuntime

  SectionIn 1 2 3 4 RO
  SetOverwrite on

  ;create folders
  CreateDirectory "$INSTDIR\$pluginDir\rmx_docs"
  CreateDirectory "$INSTDIR\$pluginDir\rmx_plugins"
  CreateDirectory "$INSTDIR\$pluginDir\rmx_scripts"
  CreateDirectory "$INSTDIR\$pluginDir\rmx_settings"
  
  ;main RMX EXE/DLL file
  SetOutPath "$INSTDIR\$pluginDir"

  !ifdef MEDIAEDITION
  File /oname=$rmxFile "${rmxdir}\gen_RMX.dll"
  StrCmp $rmxPlayer "2" "" +2
    RegDLL "$INSTDIR\$pluginDir\$rmxFile"
  !endif


  !ifdef STANDALONE
  File "${rmxdir}\rmx_automation.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RMX Automation" "UninstallString" "$INSTDIR\rmx_uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RMX Automation" "DisplayName" "RMX Automation (remove only)"
  CreateDirectory "$SMPROGRAMS\RMX Automation"
  CreateShortCut "$SMPROGRAMS\RMX Automation\RMX Automation.lnk" "$INSTDIR\rmx_automation.exe"
  CreateShortCut "$SMPROGRAMS\RMX Automation\Uninstall RMX.lnk" "$INSTDIR\rmx_uninstall.exe"
  !endif


  SetOutPath "$INSTDIR\$pluginDir\rmx_docs"
  File "..\rmx_readme.txt"

  ;Store install folder
  ;WriteRegStr HKCU "Software\${name}" "" $INSTDIR
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\$pluginDir\rmx_uninstall.exe"

SectionEnd




Section "Icon Pack" secIcon
  SectionIn 1 2 3
  SetOverwrite on
  SetOutPath "$INSTDIR\$pluginDir\rmx_icons"
  File "..\resources\icons\*.ico"
SectionEnd





SubSection /e "Device Plugins"

!ifdef STANDALONE

  Section "Power Management" SecPower
    SectionIn 1 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\power_management.dll"
  SectionEnd

  Section "Network Tools" SecNetwork
    SectionIn 1 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\network_tools.dll"
  SectionEnd

!endif

  Section "App Commands" SecAppCommand
    SectionIn 1 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\app_commands.dll"
  SectionEnd

  Section "User Input" SecKeyboard
    SectionIn 1 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\user_input.dll"
  SectionEnd

;  Section "Game Devices" SecGaming
;    SectionIn 2 3
;    SetOverwrite on
;    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
;    File "files\game_devices.dll"
;  SectionEnd

  Section "IntelliRemote" SecIntelliRemote
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\intelliremote.dll"
  SectionEnd

;  Section "Wii Remote" SecWiiRemote
;    SectionIn 2 3
;    SetOverwrite on
;    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
;    File "files\wii_remote.dll"
;  SectionEnd

  Section "Remote Center" SecRemoteCenter
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\remote_center.dll"
  SectionEnd
  
  Section "Remote Wonder" SecRemoteWonder
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\ati_remote_wonder.dll"
    ReadRegStr $R1 HKLM "SOFTWARE\ATI Technologies\Multimedia\Remote Wonder" "PlugInsPath"
    StrCmp $R1 "" errati

    SetOutPath $R1
    File "files\ati_remote_wonder.dll"
    WriteRegStr HKCU "Software\ATI Technologies\Multimedia\Remote Control\Plug-Ins\RMX Automation" "Filename" "$R1\ati_remote_wonder.dll"
    WriteRegDWORD HKCU "Software\ATI Technologies\Multimedia\Remote Control\Plug-Ins\RMX Automation" "Enabled" 1
    goto endati

    errati:
    MessageBox MB_OK|MB_ICONEXCLAMATION "ATI Remote Wonder software not found! $\nPlease install the ATI Remote Wonder software, and run this installation agian"

    endati:
  SectionEnd

  Section "Com Commander" SecComCommander
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\com_commander.dll"
  SectionEnd

;TEMPORARILY DISABLED, AS NOTHING EXPOSES PLAYLISTS CURRENTLY
;  Section "Playlist" SecPlaylist
;    SectionIn 2 3
;    SetOverwrite on
;    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
;    File "files\playlist.dll"
;  SectionEnd


  Section "RM-X10" SecX10
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\rmx10.dll"
  SectionEnd

  Section "V-Pedal" SecVPedal
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\vpedal.dll"
  SectionEnd

SubSectionEnd




SubSection /e "Optional Plugins"

;  Section "Updates" SecUpdates
;    SectionIn 1 2 3
;    SetOverwrite on
;    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
;    File "files\Updates.dll"
;  SectionEnd

  Section "Tray Icon Controller" SecTray
    SectionIn 1 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\tray_icon_controller.dll"
  SectionEnd

  Section "Toolbar Controller" SecToolbar
    SectionIn 1 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\toolbar_controller.dll"
  SectionEnd

  Section "Menu Items" SecMenu
    SectionIn 1 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\menu_items.dll"
  SectionEnd

!ifdef STANDALONE

  Section "Macro" SecMacro
    SectionIn 1 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\macro.dll"
  SectionEnd

  Section "Profiles" SecProfile
    SectionIn 1 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\profiles.dll"
  SectionEnd

  Section "Shell Extensions" SecShell
    SectionIn 1 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\shell_extensions.dll"
  SectionEnd

!endif
 
  Section "System Mixer" SecSysMixer
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\system_mixer.dll"
  SectionEnd

  Section "Telnet Daemon" SecTelnet
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\telnet_daemon.dll"
  SectionEnd

  Section "Web Server" SecWeb
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\web_server.dll"
  SectionEnd

  Section "Message Digest" SecMD5
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\message_digest.dll"
  SectionEnd

  Section "Performance" SecPerformance
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\performance.dll"
  SectionEnd

  Section "Test Plugin" SecTestPlugin
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\test_plugin.dll"
  SectionEnd

SubSectionEnd


!ifdef STANDALONE

SubSection /e "Application Plugins"

  Section "Debris" SecDebris
    SectionIn 2 3
    SetOverwrite on
    SetOutPath "$INSTDIR\$pluginDir\rmx_plugins"
    File "files\debris.dll"
  SectionEnd

SubSectionEnd

!endif




;--------------------------------
;Descriptions
  LangString DESC_SecCore ${LANG_ENGLISH} "RMX Automation Core Files - Required"
  LangString DESC_SecIntelliRemote ${LANG_ENGLISH} "For use with SoundBlaster Live! and Audigy remotes"
  LangString DESC_SecRemoteCenter ${LANG_ENGLISH} "For use with SoundBlaster Extigy and iNFRA remotes"
  LangString DESC_SecRemoteWonder ${LANG_ENGLISH} "For use with ATI RemoteWonder"
  LangString DESC_SecTestPlugin ${LANG_ENGLISH} "Test plugin to help debug RMX features"
  LangString DESC_SecGaming ${LANG_ENGLISH} "For use with DirectInput Gaming Devices, such as Gamepads, Flight Sticks, Wheels, etc"
  LangString DESC_SecKeyboard ${LANG_ENGLISH} "System-wide Keyboard Shortcuts - Also works with some multimedia keyboards"
  LangString DESC_SecTray ${LANG_ENGLISH} "System tray icon - Icon Pack recomended"
  LangString DESC_SecIcon ${LANG_ENGLISH} "A set of icon files that can be used with various plugins"
  LangString DESC_SecAppCommand ${LANG_ENGLISH} "Handler for WM_APPCOMMAND messages generated from various multimedia devices"
  LangString DESC_SecComCommander ${LANG_ENGLISH} "A 4 button remote attached to a COM port"
  LangString DESC_SecSysMixer ${LANG_ENGLISH} "Allows you to control the system volume"
  LangString DESC_SecTelnet ${LANG_ENGLISH} "Telnet server for RMX with Lua command interface"
  LangString DESC_SecPlaylist ${LANG_ENGLISH} "Displays a simple playlist window that interacts with the media player"
  LangString DESC_SecMD5 ${LANG_ENGLISH} "Message Digest (MD2, MD4, MD5) based hash code support"
  LangString DESC_SecToolbar ${LANG_ENGLISH} "Floating toolbar with customizable buttons"
  LangString DESC_SecX10 ${LANG_ENGLISH} "Sends and receives X10 powerline signals from a CM11A"
  LangString DESC_SecUpdates ${LANG_ENGLISH} "Provides automatic update support for RMX"
  LangString DESC_SecMacro ${LANG_ENGLISH} "Plugin that allows for multiple commands to be executed in a row"
  LangString DESC_SecProfile ${LANG_ENGLISH} "Plugin that allows for grouping commands together"
  LangString DESC_SecShell ${LANG_ENGLISH} "Adds the ability to interact with other applications on the system"
  LangString DESC_SecDebris ${LANG_ENGLISH} "Control Debris Visual Art by Brennan Underwood"
  LangString DESC_SecPower ${LANG_ENGLISH} "Allows for shutting down/restart system"
  LangString DESC_SecNetwork ${LANG_ENGLISH} "Provides basic networking support to RMX scripts (such as DNS)"
  LangString DESC_SecWiiRemote ${LANG_ENGLISH} "Allow control via a Nintendo Wii Remote through the use of BlueTooth"
  LangString DESC_SecVPedal ${LANG_ENGLISH} "Allow control via a V-Pedal (www.vpedal.com)"
  LangString DESC_SecWeb ${LANG_ENGLISH} "An HTTP based web server for accessing and configuring RMX"
  LangString DESC_SecPerformance ${LANG_ENGLISH} "System performance information, including FPS provided by FRAP (www.fraps.com)"
  LangString DESC_SecMenu ${LANG_ENGLISH} "Customize RMX Menu Items"


  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecCore} $(DESC_SecCore)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecIntelliRemote} $(DESC_SecIntelliRemote)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecRemoteCenter} $(DESC_SecRemoteCenter)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecRemoteWonder} $(DESC_SecRemoteWonder)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecTestPlugin} $(DESC_SecTestPlugin)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecGaming} $(DESC_SecGaming)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecKeyboard} $(DESC_SecKeyboard)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecTray} $(DESC_SecTray)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecIcon} $(DESC_SecIcon)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecComCommander} $(DESC_SecComCommander)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecAppCommand} $(DESC_SecAppCommand)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecSysMixer} $(DESC_SecSysMixer)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecTelnet} $(DESC_SecTelnet)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecPlaylist} $(DESC_SecPlaylist)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMD5} $(DESC_SecMD5)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecToolbar} $(DESC_SecToolbar)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecX10} $(DESC_SecX10)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecUpdates} $(DESC_SecUpdates)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMacro} $(DESC_SecMacro)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecProfile} $(DESC_SecProfile)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecShell} $(DESC_SecShell)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDebris} $(DESC_SecDebris)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecPower} $(DESC_SecPower)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecNetwork} $(DESC_SecNetwork)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecWiiRemote} $(DESC_SecWiiRemote)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecVPedal} $(DESC_SecVPedal)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecWeb} $(DESC_SecWeb)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecPerformance} $(DESC_SecPerformance)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMenu} $(DESC_SecMenu)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
