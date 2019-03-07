!define RUNTIME_PATH_XP  "WinSxS\x86_Microsoft.VC80.CRT_1fc8b3b9a1e18e3b_8.0.50727.42*"
!define RUNTIME_PATH_2K  "msvcr80.dll"
!define RUNTIME_URL      "http://download.microsoft.com/download/d/3/4/d342efa6-3266-4157-a2ec-5174867be706/vcredist_x86.exe"


Function DownloadRuntime
  StrCpy $2 "$TEMP\vcredist_x86.exe"
  nsisdl::download /TIMEOUT=30000 ${RUNTIME_URL} $2
  Pop $R0
    StrCmp $R0 "success" +3
    MessageBox MB_OK "Download failed: $R0"
    Quit
  ExecWait "$2 /Q"
  Delete $2
FunctionEnd


Function GetRuntime
  Push $R0
  Push $R1
  Push $R2

  ClearErrors
  ReadRegStr $R0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
  IfErrors Win2k
  StrCpy $R1 $R0 1 0
  StrCpy $R2 $R0 1 2

  StrCmp $R1 "4" Win2k
  StrCmp $R1 "5" 0 WinXP
  StrCmp $R2 "0" Win2k WinXP


;works with Windows XP, server 2003, Vista, and newer
WinXP:
  IfFileExists "$WINDIR\${RUNTIME_PATH_XP}" NextStep
  Call DownloadRuntime
  Goto NextStep

;works with Windows 2000, ME, NT4, and 9x
Win2k:
  IfFileExists "$SYSDIR\${RUNTIME_PATH_2K}" NextStep
  Call DownloadRuntime
  Goto NextStep


NextStep:
  Pop $R2
  Pop $R1
  Pop $R0
FunctionEnd