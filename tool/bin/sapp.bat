@echo off

set BAT_PATH=%~dp0
set BAT_PATH=%BAT_PATH:~0,-1%

"%BAT_PATH%\..\..\bin\Windows\sapp.exe" %*
