@echo off

set SLIB_PATH=%~dp0
set SLIB_PATH=%SLIB_PATH:~0,-1%

echo SLIB_PATH=%SLIB_PATH%

echo Registering environment variable
setx SLIB_PATH "%SLIB_PATH%"

for /F "usebackq tokens=2*" %%A in (`REG QUERY HKCU\Environment /v Path`) do set newPath=%%B
echo "%newPath%"|find /i "%SLIB_PATH%/tool/bin">nul || goto register_path
echo Tool path is already registered in Path
goto register_path_end

:register_path
if "%newPath:~-1%"==";" goto register_path_do
set newPath=%newPath%;

:register_path_do
echo Registering tool path in Path
set newPath=%newPath%%%SLIB_PATH%%/tool/bin;
setx Path "%newPath%"

:register_path_end
if "%1"=="quick" goto :eof
timeout 5
