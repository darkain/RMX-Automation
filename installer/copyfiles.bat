@ECHO OFF


del files\*.exe
del files\*.dll


copy /B "\Program Files\Winamp\Plugins\gen_RMX.dll" files
copy /B "..\testbed\rmx_automation.exe" files
copy /B "..\testbed\rmx_plugins\*.dll" files


"c:\Program Files\NSIS\makensis.exe" rmx_media_edition.nsi
"c:\Program Files\NSIS\makensis.exe" rmx_stand_alone.nsi


PAUSE