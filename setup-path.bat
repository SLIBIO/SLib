@echo off

set SLIB_PATH=%~dp0
set SLIB_PATH=%SLIB_PATH:~0,-1%

echo SLIB_PATH=%SLIB_PATH%

echo Registering environment variable
setx SLIB_PATH %SLIB_PATH%

timeout 5
