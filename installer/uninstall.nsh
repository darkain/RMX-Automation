Section "Uninstall"
  Delete "$INSTDIR\rmx_docs\*.*"
  Delete "$INSTDIR\rmx_icons\*.*"
  Delete "$INSTDIR\rmx_plugins\*.*"
  Delete "$INSTDIR\rmx_scripts\*.*"
  Delete "$INSTDIR\rmx_settings\*.*"
  Delete "$INSTDIR\rmx_web\*.*"

  RMDir "$INSTDIR\rmx_docs"
  RMDir "$INSTDIR\rmx_icons"
  RMDir "$INSTDIR\rmx_plugins"
  RMDir "$INSTDIR\rmx_scripts"
  RMDir "$INSTDIR\rmx_settings"
  RMDir "$INSTDIR\rmx_web"

!ifdef MEDIAEDITION
  ;todo: add IfFileExists
  UnRegDLL "$INSTDIR\wmp_RMX.dll"

  Delete "$INSTDIR\gen_RMX.dll"
  Delete "$INSTDIR\wmp_RMX.dll"
  Delete "$INSTDIR\foo_RMX.dll"
  Delete "$INSTDIR\rl_RMX.dll"
  Delete "$INSTDIR\bs_RMX.dll"

  Delete "$INSTDIR\gen_RMX.dll.old"
  Delete "$INSTDIR\wmp_RMX.dll.old"
  Delete "$INSTDIR\foo_RMX.dll.old"
  Delete "$INSTDIR\rl_RMX.dll.old"
  Delete "$INSTDIR\bs_RMX.dll.old"
!endif

!ifdef STANDALONE
  Delete "$SMPROGRAMS\RMX Automation\RMX Automation.lnk"
  Delete "$SMPROGRAMS\RMX Automation\Uninstall RMX.lnk"
  RMDir  "$SMPROGRAMS\RMX Automation"
  Delete "$INSTDIR\rmx_automation.exe"
  Delete "$INSTDIR\rmx_automation.exe.old"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\RMX Automation"
  ;DeleteRegKey /ifempty HKCU "Software\${name}"
!endif

  Delete "$INSTDIR\rmx_uninstall.exe"
  RmDir "$INSTDIR"
SectionEnd
